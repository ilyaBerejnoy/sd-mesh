В файлах sd_mesh.c and sd_mesh.h реализуется функционал регистрации и маршрутизации в самоорганизуещейся сети SD-Mesh.

Файл sd_mesh.c содержит реализацию апи для приложения, которое будет работать поверх данной сети.

Код реализован для стабильной работы как во встроенном приложении, так и в 
управляемом через события коде. Вызывающая программа(клиент), работающая в цикле или в ввыделенном потоке,
должна периодически вызывать функцию sdm_handler(). Также данная функция должна вызываться всякий раз,
когда приходят данные из сети.

все функции по-умолчанию возвращают -1 в случае ошибки, в случае успеха 1. Код последней ошибки возвращает функция 
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


* sdm_hello

Данная функция отправляет широковещательный запрос ближашим соседям. В ответ все ближайшие соседи отправляют данные о себе


* sdm_ping

Данная функция предназанчена для проверки нахождения узла в сети и в случае успеха данный узел будет или
добавлен в таблицу рутинга или обновлен статус получения ответа
Входные параметры:
IPV6_TYPE uuid - ID искомого узла


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
in_buf, in_buf_len - указатель на буфер данных и его размер. Если за истекшее время данные никакие данные не пришли,
то необходимо передать указатель на буфер или длину буфера равными 0.

Выходные параметры:
out_sleep - рекомендованное время, после которого sdm_handler должна быть вызвана вновь в миллисекундах.


* sdm_search_peer

Поиск пути к выбранному узлу. Возвращает хэндл запроса или -1 в случае ошибки. Может занимать некоторое время, статус отслеживается через вызов функции sdm_get_search_peer_status. 
Одновременно может быть инициировано до SMD_PEER_MAX_SEARCHES (128) запросов, при попытке инициировать новый запрос функция вернет -1.

Входные параметры:
PV6_TYPE uuid - ID узла;

* sdm_get_search_peer_status
Возвращает текущий статус процесса поиска узла. 

Входные параметры:
UINT request_id - хэндл запроса;



Information queries
*******************

* sdm_get_nodes_cnt

Ворзвращает число нод (суперузлов) в таблице рутинга.


* sdm_get_nodes

возвращает список проверенных, надежных и наиболее часто используемых нод(суперузлов). Хорошой практикой будет пропинговать их перед выключением и после запуска устройства или приложения.


* sdm_blacklisted

Возврашает 1, если запрос с указанного IP адреса должен игнорироваться, иначе -1.

Входные параметры:
PV4_TYPE ip - ip адрес узла;

* sdm_get_last_errno

Возвращает код последней ошибки, возникшей при работе либы


* sdm_dump_tables

Выводит в поток визуальное представление таблицы рутинга.

Functions provided by you
*************************

* Функции для регстрации колбеков

* sdm_reg_failed_node_request_callback

Входные параметры:
FAILED_NODE_REQUEST_FUNC_TYPE customClbck

Зарегистрированный колбек вызывается в случае неудачи при поиске пира.
Если узел не смог найти таргет узел по своей таблице узлов и суперузлы не дали ни одного адреса, то библиотека вызвает эту функцию для сигнализации о проблеме выше приложению и приложение запросило поиск маршрута в центральном сервере.

* sdm_reg_send_data_callback

Зарегистрированный колбек вызывается в случае, когда бибиотеке нужно отправить данные по сети.

SEND_DATA2NODES_FUNC_TYPE sendToClbck

Final notes
***********

The best is yet to come!


                                        Ilya Berezhnoy
                                        <berezhnoy.ilya@gmail.com>
