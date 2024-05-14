# Buffered channel

#### Eng

Here is an implementation of one of the basic structures in the Go language - a channel (go channel).
A channel allows organizing data transmission from a sender to a receiver (for example, from one function to another), i.e. it supports the following operations:

* _send(value)_ --- send value through the channel.
* _recv()_ --- receive the next value from the channel.
* _close()_ --- close the channel for transmission and reception.

The most important feature of such a channel is the ability for multiple senders to write data to one channel simultaneously and multiple receivers to receive them without any additional synchronization from their side. This allows conveniently organizing computational pipelines - each function in the pipeline receives data from one channel and writes the result of its processing to another channel for the next function. Moreover, each function can be executed by multiple threads simultaneously, and the developer doesn't even need to think about parallelizing computations (managing data distribution between threads, ensuring synchronization) - the channel handles all this work.

Such a channel has a defined _capacity_, let's denote it as $`n`$, and the channel itself maintains a FIFO queue of size not exceeding $`n`$, and operates as follows:

* _send(value)_ --- if the current number of elements in the queue equals $n$ (i.e., the queue is full), then the thread blocks until there is free space in the queue. After that, the `value` is written to the end of the queue.
* _recv()_ --- if the queue is empty, the thread blocks until something is written to the queue. After that, the next value is extracted from the head of the queue and returned as a result. Thus, each value can only be received once, but the receiving thread (from those that performed `recv`) can be any.
* _close()_ --- closes the channel. If there are still some elements in the queue, the channel continues to send them. However, further attempts to `send` after closing the channel result in an error. `recv`, after closing, reads the remaining elements in the queue, and subsequent calls should return a value signaling the closure of the channel.

#### Rus

Здесь представлена реализация одной из базовых структур языка Go --- канала (go channel).
Канал позволяет организовать передачу данных от приемника к источнику (например, от одной функции к другой), т.е. поддерживает следующие операции:
* _send(value)_ --- отправить value через канал.
* _recv()_ --- получить очередное значение из канала.
* _close()_ --- закрыть канал для передачи и приема.

Важнейшей особенностью такого канала является возможность сразу нескольких источников писать в один канал данные и сразу нескольким получателям их принимать без всякой дополнительной синхронизации с их стороны. Это позволяет удобным образом организовывать вычислительные конвейеры --- каждая функция в конвейере принимает данные из одного канала и пишет результат их обработки в другой канал для следующей функции. При этом vкаждая функция может исполняться сразу несколькими потоками, а разработчику даже не нужно думать о распараллеливании вычислений (заниматься распределением данных между потоками, следить за синхронизацией) --- всю эту работу канал берет на себя.

У такого канала определена _емкость_, обозначим ее за $`n`$, а сам канал поддерживает очередь FIFO размера не более $`n`$ и работает следующим образом:
* _send(value)_ --- если текущее количество элементов в очереди равно $`n`$ (т.е. очередь заполнена), то поток блокируется до тех пор, пока в очереди не появится свободное место. После этого `value` записывается в конец очереди.
* _recv()_ --- если очередь пуста, то поток блокируется до тех пор, пока в очередь не будет что-нибудь записано. После этого из головы очереди извлекается очередное значение и возвращается как результат. Таким образом, каждое значение может быть получено только в 1 экземпляре, но получающий поток (из тех, что сделали `recv`) может быть любым.
* _close()_ --- закрывает канал. Если в очереди еще содержатся какие-то элементы, то канал продолжает заниматься их отправкой. Но прочие попытки сделать `send` после закрытия канала приводят к ошибке. `recv` же после закрытия дочитывает оставшиеся в очереди элементы, а дальнейшие вызовы должны возвращать значение, сигнализирующее о закрытии канала.
