# Readers–writer lock

#### Eng

One of the classical problems in parallel programming is the [reader-writer problem](https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem). Its essence lies in having a shared resource, with some threads only reading from it (without making changes), while others write to it. Concurrent reading can be performed by multiple threads simultaneously, but only one thread can write at a time.

The RW-lock is one of the synchronization primitives that solve this problem. Since many resources (such as the entire STL) support concurrent reading (but only exclusive writing), using an RW-lock instead of a mutex in such cases can improve your program's performance. It can be said that either one writing thread or multiple reading threads can enter the critical section in an RW-lock.

The file `rw_lock.h` contains an implementation of this primitive. The `read` and `write` functions take callback functions, which perform the actual reading/writing work after entering the critical section.

#### Rus

Одной из классических задач параллельного программирования является [задача о читателях-писателях](https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem).
Суть ее в том, что есть некий общий ресурс, и какие-то потоки используют его только в режиме чтения (не внося изменений), а какие-то --- в режиме записи.
При этом одновременно производить чтение могут сразу несколько потоков, а вот писать только один.

RW-lock --- один из примитивов синхронизации, решающий эту задачу. Поскольку многие ресурсы (например, весь STL) поддерживают одновременное чтение (но только уникальную запись), использование rw-lock вместо мьютекса в таких случаях может улучшить производительность вашей программы. Можно сказать, что в критическуюсекцию в rw-lock могут войти либо один пишущий поток, либо несколько читающих.

В файле `rw_lock.h` приведена реализация данного примитива. `read` и `write` принимают функции обратного вызова, которые и выполняют работу по чтению/записи после прохода в критическую секцию.