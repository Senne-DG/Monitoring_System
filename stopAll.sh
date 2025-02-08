#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "Stopping loggerServer..."
"$SCRIPT_DIR/loggerServer.sh" stop

echo "Stopping monitoringSystem..."
"$SCRIPT_DIR/monitoringSystem.sh" stop

# echo "Stopping sensorClient..."
# "$SCRIPT_DIR/sensorClient.sh" stop

echo "All processes have been stopped."
