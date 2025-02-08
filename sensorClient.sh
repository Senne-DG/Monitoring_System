#!/bin/bash

APP_NAME="sensorClient"

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR="$SCRIPT_DIR/build"
APP_EXEC="$BUILD_DIR/$APP_NAME"

PID_FILE="/tmp/${APP_NAME}.pid"

start() {
    if [ -f "$PID_FILE" ]; then
        PID=$(cat "$PID_FILE")
        if kill -0 "$PID" 2>/dev/null; then
            echo "$APP_NAME is already running with PID $PID."
            exit 1
        else
            echo "Stale PID file found. Removing..."
            rm -f "$PID_FILE"
        fi
    fi

    echo "Starting $APP_NAME with arguments: $@"
    # Forward all extra arguments to the executable.
    $APP_EXEC "$@" &
    PID=$!
    echo $PID > "$PID_FILE"
    echo "$APP_NAME started with PID $PID."
}

stop() {
    if [ ! -f "$PID_FILE" ]; then
        echo "$APP_NAME is not running (no PID file found)."
        exit 1
    fi
    PID=$(cat "$PID_FILE")
    if kill -0 "$PID" 2>/dev/null; then
        echo "Stopping $APP_NAME with PID $PID..."
        kill "$PID"
        sleep 1
        if kill -0 "$PID" 2>/dev/null; then
            echo "$APP_NAME did not exit, forcing termination..."
            kill -9 "$PID"
        fi
        rm -f "$PID_FILE"
        echo "$APP_NAME stopped."
    else
        echo "No process with PID $PID found. Removing stale PID file."
        rm -f "$PID_FILE"
    fi
}

status() {
    if [ -f "$PID_FILE" ]; then
        PID=$(cat "$PID_FILE")
        if kill -0 "$PID" 2>/dev/null; then
            echo "$APP_NAME is running with PID $PID."
        else
            echo "$APP_NAME is not running, but PID file exists."
        fi
    else
        echo "$APP_NAME is not running."
    fi
}

restart() {
    stop
    start "$@"
}

# Parse the action command.
case "$1" in
    start)
        # Remove the first argument and pass the rest (like "-p 8080") to the start function.
        shift
        start "$@"
        ;;
    stop)
        stop
        ;;
    status)
        status
        ;;
    restart)
        shift
        restart "$@"
        ;;
    *)
        echo "Usage: $0 {start|stop|status|restart} [extra arguments]"
        exit 1
        ;;
esac
