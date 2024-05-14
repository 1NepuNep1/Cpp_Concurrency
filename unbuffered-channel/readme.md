# Unbuffered channel

#### Eng

Here is an implementation of an unbuffered channel in the Go language.

Its operations differ as follows:

* _send(value)_ --- the thread blocks until the receiving end receives the corresponding _value_.
* _recv()_ --- the thread blocks until there is any _send_ available to be received.
* _close()_ --- closes the channel. If there are any pending _send_ operations, they and all subsequent _send_ attempts should result in an error. All _recv_ calls (both subsequent and current ones waiting) should return a value indicating that the channel is closed.

#### Rus

Здесь представлена реализация небуферизированного канала из языка go.

Его операции отличаются следующим:
* _send(value)_ --- поток блокируется до тех пор, пока принимающая сторона не получит соответствующее _value_.
* _recv()_ --- поток блокируется до тех пор, пока не появится какой-либо _send_, который можно принять.
* _close()_ --- закрыть канал. Если при этом есть какие-то ожидающие _send_ потоки, то и они, и все последующие попытки _send_ должны завершаться с ошибкой.
А все вызовы _recv_ (как последующие, так и текущие ожидающие) должны возвращать значение, свидетельствующее о том, что канал закрыт.
