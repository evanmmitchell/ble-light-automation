import json
from time import time

import numpy as np
import paho.mqtt.client as mqtt
import pandas as pd
import requests
from sklearn.linear_model import SGDClassifier

from secrets import HOME_ASSISTANT_AUTH


LIGHT_ENTITIES = {"bedroom": ["switch.bedroom_light"], "living_room": ["switch.string_lights", "switch.rose"]}
ROOMS = list(LIGHT_ENTITIES)

HOME_ASSISTANT_API = "http://localhost:8123/api/"
HEADERS = {"Authorization": f"Bearer {HOME_ASSISTANT_AUTH}", "Content-Type": "application/json"}
MQTT_BROKER = "localhost"

NUM_BLE_DEVICES = 250


def on_connect(client, userdata, flags, rc):
    client.subscribe("home-assistant/ble-light-automation", 1)
client = mqtt.Client()
client.on_connect = on_connect
client.connect(MQTT_BROKER)


classifier = SGDClassifier(loss="log", learning_rate="constant", eta0=0.5)
init_weight = None
last_seen_ble_devices = {}
stored_messages = {room: [] for room in ROOMS}
stored_messages_index = {room: 0 for room in ROOMS}
store = True
switch_timer = 0


lights_on = {room: False for room in ROOMS}
was_sunset = True
sunset = False


def train_classify(train, ble_rssi, room):
    global was_sunset
    global init_weight
    global switch_timer
    global sunset

    if sunset:
        print("SUNSET")
        if len(room) == 0:
            train = 0
        was_sunset = True
        sunset = False

    if train == 1:
        print("TRAINING")
    else:
        print("CLASSIFYING")

    if train == 1:
        # Update last n seen BLE devices
        for ble in ble_rssi.keys():
            try:
                last_seen_ble_devices.pop(ble)
            except KeyError:
                pass
            last_seen_ble_devices[ble] = None
        num_to_remove = len(last_seen_ble_devices) - NUM_BLE_DEVICES
        if num_to_remove > 0:
            for ble in list(last_seen_ble_devices)[0:num_to_remove]:
                last_seen_ble_devices.pop(ble)

    # Format sample
    sample = pd.DataFrame(np.nan, index=[0], columns=[str(i) for i in range(NUM_BLE_DEVICES)])
    sample = sample.rename(dict(zip(sample.columns.values[0:len(last_seen_ble_devices)], sorted(last_seen_ble_devices.keys()))), axis=1)
    sample.update(pd.DataFrame(ble_rssi, index=[0], columns=sorted(last_seen_ble_devices.keys())))
    sample = sample.applymap(lambda x: pow(10, (70+x)/100)).fillna(0)


    if train == 1:
        # Format weights
        try:
            if init_weight == None:
                init_weight = classifier.coef_[0][-1]
            old_weights = pd.DataFrame(classifier.coef_, columns=classifier.feature_names_in_)
            new_weights = pd.DataFrame(np.nan, index=[0], columns=sample.columns.values)
            new_weights.update(old_weights)
            new_weights = new_weights.fillna(init_weight)
            classifier.coef_ = new_weights.to_numpy()
            classifier.feature_names_in_ = new_weights.columns
        except AttributeError:
            pass


    if train == 1:
        # Train
        classifier.partial_fit(sample, room, ROOMS)
    else:
        # Classify
        try:
            prob = classifier.predict_proba(sample)[0]
            print(ROOMS)
            print(prob)
            if prob[ROOMS.index(room[0])] < 0.95:
                switch_timer = time() + 30
                for room in ROOMS:
                    light_on = not lights_on[room]
                    for light_entity in LIGHT_ENTITIES[room]:
                        requests.post(HOME_ASSISTANT_API + "services/switch/turn_" + ("on" if light_on else "off"), headers=HEADERS, json={"entity_id": light_entity})
        except:
            pass


def on_message(client, userdata, msg):
    global store

    print("RECEIVED")
    received = msg.payload.decode("utf-8")
    train, ble_rssi = json.loads(received)
    current_rooms = [room for room, light_on in lights_on.items() if light_on]

    if train == 1 and len(current_rooms) == 1 and store:
        print("STORED")
        stored_messages[current_rooms[0]].append(ble_rssi)
        if len(stored_messages[current_rooms[0]]) > 60:
            stored_messages[current_rooms[0]].pop(0)

    if len(current_rooms) == 1 or (sunset and len(current_rooms) == 0):
        other_rooms = [room for room in ROOMS if room not in current_rooms]
        if train == 0 and len(stored_messages[current_rooms[0]]) > 0 and len(stored_messages[other_rooms[0]]) > 0 and time() > switch_timer:
            # Classify
            train_classify(train, ble_rssi, current_rooms)
        elif train == 1 and len(stored_messages[other_rooms[0]]) > 0 and not store:
            # Train with new data and stored data
            train_classify(train, ble_rssi, current_rooms)
            stored_messages_index[other_rooms[0]] %= len(stored_messages[other_rooms[0]])
            ble_rssi = stored_messages[other_rooms[0]][stored_messages_index[other_rooms[0]]]
            stored_messages_index[other_rooms[0]] += 1
            train_classify(1, ble_rssi, other_rooms)

    store = not store

    print()

client.on_message = on_message


while True:
    for room in ROOMS:
        light_on = True
        for light_entity in LIGHT_ENTITIES[room]:
            response = requests.get(HOME_ASSISTANT_API + "states/" + light_entity, headers=HEADERS).json()
            if response["state"] == "off":
                light_on = False
        lights_on[room] = light_on

    response = requests.get(HOME_ASSISTANT_API + "states/sun.sun", headers=HEADERS).json()
    if response["state"] == "above_horizon":
        was_sunset = False
    if response["state"] == "below_horizon" and not was_sunset:
        sunset = True

    client.loop()
