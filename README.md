 sd_mesh.c and sd_mesh.h реализуют логику регистрации и маршрутизации
 в самоорганизуещейся сети SD-Mesh.

Файл sd_mesh.c содержит реализацию доступного апи для приложения,
которое будет работать поверх данной сети.

Код реализован для стабильной работы как во встроенном приложении, так и в 
управляемом через события коде. Вызывающая прогшрамма(клиент), работающая в цикле или в ввыделенном потоке,
должна периодически вызывать функцию sdm_handler(). Также данная функция должна вызываться всякий раз,
когда приходят данные из сети.

все функции возвращают -1 в случае ошибки, в случае успеха 1. Код последней ошибки возвращает функция 
sdm_get_last_errno().

Initialisation
**************

* sdm_init

Должна вызываться перед началом использования библиотеки. 
Входные параметры:
IPV6_TYPE uuid - ID узла. Должен быть уныкальным в данной сети, может быть создан фукнцией generate_peer_uuid;
IPV4_TYPE ip - статический ip адрес узла, если приложение работает за NATом (STUN, TURN); 
PORT_TYPE port - порт, на который будут прихожить все данные из сети для управления рутингом. 

* sdm_send_sys_info

Данная функция принимает основные характеристики устройства для расчета его производительности.
Сами данные передаются на центральный сервер, где реализвана логика расчета коэфициента производительности
на основе общей статистики и времени нахождения узлов в системе. 

 
* sdm_uninit

Данная функция должная вызываться, когда вызывающая программа прекращает работу в данной сети. 
После вызова данной функции для возобноваления работы потребуется пройти процедуру заново.

Bootstrapping
*************

Для начала работы в сети неободимо получить какое-то число доступных узлов для инициализация таблицы рутинга.


* sdm_echo

Данная функция отправляет широковещательный запрос ближашим соседям. В ответ все ближайшие соседи


* sdm_ping

Данная функция предназанчена для проверки нахождения узла в сети и в случае успеха данный узел будет или
добавлен в таблицу рутинга или обновлен статус получения ответа
Входные параметры:
IPV6_TYPE uuid - ID искомого узла


This is the main bootstrapping primitive.  You pass it an address at which
you believe that a DHT node may be living, and a query will be sent.  If
a node replies, and if there is space in the routing table, it will be
inserted.

* sdm_insert_node

Эта функция добавляет указанный узел в таблицу рутинга без отправки соответствующего сетевого запроса

Входные параметры:
PV6_TYPE uuid - ID узла;
IPV4_TYPE ip - статический ip адрес узла; 
PORT_TYPE port - порт узла. 


Doing some work
***************

* sdm_handler

Данная функция должна вызываться в основном цикле программы или в обработчике таймера или 
в случае нахождения данных в сокете. Рекомендованное время, после которого sdm_handler должна быть вызвана 
в случае отсуствия принятых данных возвращается в выходном параметре out_sleep.

Входные параметры:
in_buf, in_buflen - указатель на буфер данных и его размер. Если за истекшее время данные никакие данные не пришли,
то необходимо передать указатель на буфер или длину буфера равными 0.
in_callback - колбек на 
Выходные параметры:
out_sleep - рекомендованное время, после которого sdm_handler должна быть вызвана вновь в миллисекундах.


* sdm_search_peer

Поиск пути к выбранному узлу. 

Возвращает идентификатор запроса или -1 в случае ошибки.


* sdm_get_search_peer_result
возвращает статус процедуры поиска узла.
This schedules a search for information about the info-hash specified in
id; it returns 1 if this is a new search, and 0 if it merely reset the
timeouts for a search in progress.  If port is not 0, it specifies the TCP
port on which the current peer is listening; in that case, when the search
is complete it will be announced to the network.  The port is in host
order, beware if you got it from a struct sockaddr_in.

In either case, data is passed to the callback function as soon as it is
available, possibly in multiple pieces.  The callback function will also
be called when the search is complete.

Up to DHT_MAX_SEARCHES (1024) searches can be in progress at a given time;
any more, and dht_search will return -1.  If you specify a new search for
the same info hash as a search still in progress, the previous search is
combined with the new one -- you will only receive a completion indication
once.

Information queries
*******************

* dht_nodes

This returns the number of known good, dubious and cached nodes in our
routing table.  This can be used to decide whether it's reasonable to start
a search; a search is likely to be successful as long as we have a few good
nodes; however, in order to avoid overloading your bootstrap nodes, you may
want to wait until good is at least 4 and good + doubtful is at least 30 or
so.

It also includes the number of nodes that recently sent us an unsolicited
request; this can be used to determine if the UDP port used for the DHT is
firewalled.

If you want to display a single figure to the user, you should display
good + doubtful, which is the total number of nodes in your routing table.
Some clients try to estimate the total number of nodes, but this doesn't
make much sense -- since the result is exponential in the number of nodes
in the routing table, small variations in the latter cause huge jumps in
the former.

* dht_get_nodes

This retrieves the list of known good nodes, starting with the nodes in our
own bucket.  It is a good idea to save the list of known good nodes at
shutdown, and ping them at startup.

* dht_dump_tables
* dht_debug

These are debugging aids.

Functions provided by you
*************************

* The callback function

The callback function is called with 5 arguments.  Closure is simply the
value that you passed to dht_periodic.  Event is one of DHT_EVENT_VALUES or
DHT_EVENT_VALUES6, which indicates that we have new values, or
DHT_EVENT_SEARCH_DONE or DHT_EVENT_SEARCH_DONE6, which indicates that
a search has completed.  In either case, info_hash is set to the info-hash
of the search.

In the case of DHT_EVENT_VALUES, data is a list of nodes in ``compact''
format -- 6 or 18 bytes per node.  Its length in bytes is in data_len.

* dht_sendto

This will be called whenever the library needs to send a datagram.  If the
integers passed to dht_init are file descriptors, this can simply be an
alias for the sendto system call.

* dht_blacklisted

This is a function that takes an IP address and returns true if this
address should be silently ignored.  Do not use this feature unless you
really must -- Kademlia supposes transitive reachability.

* dht_hash

This should compute a reasonably strong cryptographic hash of the passed
values.  SHA-1 should be good enough.

* dht_random_bytes

This should fill the supplied buffer with cryptographically strong random
bytes.  It's called every 30 minutes on average, so it doesn't need to be
fast.

Final notes
***********

* NAT

Nothing works well across NATs, but Kademlia is somewhat less impacted than
many other protocols.  The implementation takes care to distinguish between
unidirectional and bidirectional reachability, and NATed nodes will
eventually fall out from other nodes' routing tables.

While there is no periodic pinging in this implementation, maintaining
a full routing table requires slightly more than one packet exchange per
minute, even in a completely idle network; this should be sufficient to
make most full cone NATs happy.


                                        Juliusz Chroboczek
                                        <jch@pps.jussieu.fr>
