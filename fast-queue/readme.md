# Multiple producer multiple consumer bounded queue

#### Eng

Here is an implementation of one of the fast bounded queues: MPMC queue. MP stands for multiple producers, meaning multiple data producers can simultaneously use the queue to write data. MC stands for multiple consumers, meaning multiple data consumers can simultaneously access the data.

The queue behaves as follows:
* `enqueue` returns `false` if the queue is full; otherwise, it inserts the element and returns `true`.
* `dequeue` returns `false` if the queue is empty; otherwise, it retrieves the element and returns `true`.

#### Rus

Здесь представлена реализация одной из быстрых очередей ограниченного размера: MPMC очереди. MP означает multiple producer, т.е. возможно одновременно использовать несколько производителей данных, которые записывают данные в очередь. MC --- multiple consumer, т.е. возможно несколько потребителей данных одновременно.

Очередь ведет себя так:
* `enqueue` возвращает `false` если очередь заполнена, в противном случае вставляет элемент и возвращает `true`.
* `dequeue` возвращает `false` если очередь пуста, в противном случае достает элемент и возвращает `true`.

