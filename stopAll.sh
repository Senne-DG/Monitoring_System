#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

"$SCRIPT_DIR/loggerServer.sh" stop
"$SCRIPT_DIR/monitoringSystem.sh" stop
"$SCRIPT_DIR/sensorClient.sh" stop

echo "All processes have been stopped."
