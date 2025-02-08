#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

"$SCRIPT_DIR/loggerServer.sh" start
"$SCRIPT_DIR/monitoringSystem.sh" start

# Maybe this should be seperate because it is the clientside
# "$SCRIPT_DIR/sensorClient.sh" start -p 12345

echo "All processes have been started."
