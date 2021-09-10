#!/bin/bash

date
whoami
hostnamectl --static

echo "Процессор:"
echo " • Модель - $(lscpu | grep -oP "Имя модели:\s*\K.+")"
echo " • Архитектура - $(lscpu | grep -oP "Архитектура:\s*\K.+")"
echo " • Тактовая частота - $(lscpu | grep -oP "CPU max MHz:\s*\K.+")"
cores_per_socket=$(lscpu | grep -oP "Ядер на сокет:\s*\K.+")
sockets_count=$(lscpu | grep -oP "Сокетов:\s*\K.+")
echo " • Количество ядер - $(($cores_per_socket * $sockets_count))"
echo " • Количество потоков на одно ядро - $(lscpu | grep -oP -i 'Thread\(s\) per core:\s*\K.+')"

echo "Оперативная память:"
echo " • Всего – $(cat /proc/meminfo | grep -oP "MemTotal:\s*\K.+")"
echo " • Доступно – $(cat /proc/meminfo | grep -oP "MemFree:\s*\K.+")"

echo "Жесткий диск:"
echo " • Всего – $(($(lsblk -b --output SIZE -n -d /dev/sda) / $1024)) kB"
echo " • Доступно – $(($(lsblk -b --output SIZE -n -d /dev/sda3) / $1024)) kB"
echo " • Смонтировано в корень / – $(($(lsblk -b --output SIZE -n -d /dev/sda2) / $1024)) kB"
echo " • Swap всего – $(cat /proc/meminfo | grep -oP "SwapTotal:\s*\K.+")"
echo " • Swap доступно – $(cat /proc/meminfo | grep -oP "SwapFree:\s*\K.+")"

echo "Сетевые интерфейсы:"
echo " • Количество сетевых интерфейсов - $(wc -w <<< $(ls -1 /sys/class/net))"
printf "\n%s\t%10s\t%20s\t%25s\t%40s\n" "№" "Имя сетевого интерфейса" "MAC адрес" "IP адрес" "Скорость соединения"
count=1
for s in $(ls -1 /sys/class/net)
do
	printf "%d\t%15s\t%27s\t%15s" $count $s $(cat /sys/class/net/$s/address) $(ip -br a show | grep $s | gawk '{print $3}')
	printf "%18s\n" $(sudo ethtool $s | grep -oP "Speed:\s*\K.+")
	((count++))
done
