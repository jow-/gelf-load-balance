#!/bin/sh


SERVER_NAME=gelf-load-balance
APP_HOME=`echo $0 | sed -e 's@/[^/]*$@@'`
if [ "$APP_HOME" = '.' ]; then
    APP_HOME=$(echo `pwd`)
fi

if [ ! -d $APP_HOME/pids ]; then
    mkdir $APP_HOME/pids
fi

PIDFILE=$APP_HOME/pids/$SERVER_NAME.pid

start() {
    if [ -e $PIDFILE ];
    then
        if kill -0 `cat $PIDFILE` > /dev/null 2>&1
        then
            echo "$SERVER_NAME service already running."
            exit 1
        fi
    else
        touch $PIDFILE
    fi

    if [ -e $JAR ];
    then
        echo "starting server."
        nohup $SERVER_NAME >/dev/null 2>&1 &
        echo $! > $PIDFILE
    else
        echo "Can't find the executable jar file."
        exit 1
    fi
}

stop(){
    if [ -e $PIDFILE ];
    then
        echo "stopping server"
        if kill -TERM `cat "$PIDFILE"`
        then
            sleep 2
            loop_check_process_status 3
        elif
            kill -KILL `cat "$PIDFILE"`
        then
            sleep 2
            loop_check_process_status 3
        elif
            kill -9 `cat "$PIDFILE"`
        then
            echo "server stop OK"
            rm $PIDFILE
        fi
    else
        echo "Server not running."
        exit 1
    fi
}

restart(){
    echo "Restarting server."
    stop
    start $1
}

loop_check_process_status() {
    COUNT=$1
    KILLED=false
    for ((i=0; i < $COUNT; i++));do
        if [ -d $APP_HOME/`cat $PIDFILE` ];
        then
            sleep 10
        else
            KILLED=true
            break
        fi
    done
    if $KILLED;then
        echo "server stopped."
        rm $PIDFILE
    else
        echo "Stopping server failed."
    fi
}

case $1 in
    start)
        start $2
        ;;
    stop)
        stop
        ;;
    restart)
        restart $2
        ;;
    *)
        echo "Usage: $0 {start|stop|restart}" >&2
        ;;
esac

exit 0

