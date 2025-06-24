#!/bin/sh

# Функция для получения информации о потоках процесса
get_process_thread_info() {
    local process_name="$1"
    local pid=$(pgrep -x "$process_name")

    if [ -n "$pid" ]; then
        local threads=$(ps -o nlwp= -p "$pid" | tr -d ' ')
        local max_threads=$(ps -o thcount= -p "$pid" | tr -d ' ')
        echo "$pid $threads $max_threads"
    else
        echo "0 0 0"
    fi
}

last_pid=0
max_threads=0

clear
printf "Процесс FFTBenchmark не найден\n"

while true; do
    # Получаем информацию о процессе FFTBenchmark
    process_thread_info=$(get_process_thread_info FFTBenchmark)
    pid=$(echo "$process_thread_info" | awk '{print $1}')
    pthreads=$(echo "$process_thread_info" | awk '{print $2}')
    pmax_threads=$(echo "$process_thread_info" | awk '{print $3}')

    # Если процесс найден
    if [ "$pid" -ne 0 ]; then
        last_pid=$pid

        # Обновляем максимальное количество потоков
        if [ "$pmax_threads" -gt "$max_threads" ]; then
            max_threads=$pmax_threads
        fi

        clear
        printf "Найден процесс FFTBenchmark (PID: %d)\nТекущее количество потоков: %d\nМаксимальное число потоков: %d\n" \
               "$pid" "$pthreads" "$max_threads"
    elif [ "$last_pid" -ne 0 ]; then
        printf "Процесс FFTBenchmark завершен (PID: %d)\n" "$last_pid"
        last_pid=0
        max_threads=0
    fi

    sleep 0.1
done