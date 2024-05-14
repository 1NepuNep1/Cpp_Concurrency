# timerqueue

#### Eng

Here is the implementation of the synchronization primitive `TimerQueue<T>`.

The TimerQueue stores a set of pairs `(v, t)`, where `v` is the value and `t` is the time.

The `Add(v, t)` operation adds a new pair to the set.

The `Pop()` operation blocks until the `minimum t among all elements in the set` is reached, then it returns the element with the minimum `t` (and removes it from the set). If the set is empty, `Pop()` waits indefinitely.

#### Rus

Здесь представлена реализация примитива синхронизации `TimerQueue<T>`.

TimerQueue хранит внутри себя множество пар `(v, t)`. `v` - это значение, `t` - это момент времени.

Операция `Add(v, t)` добавляет новую пару в множество.

Операция `Pop()` блокируется до момента `min(t) по всем элементам множества` и затем возвращает элемент с минимальным `t` (при этом, удаляя его их множества). Если множество пустое, `Pop()` ждёт вечно.