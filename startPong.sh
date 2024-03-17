#!/bin/bash

./build/Pong server &
SERVER_PID=$!

sleep 1

./build/Pong client

kill $SERVER_PID

