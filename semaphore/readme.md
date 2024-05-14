# Semaphore

#### Eng

Here is an improved implementation of one of the basic synchronization primitives - the semaphore. Unlike a mutex, a semaphore allows not just one but $`n`$ threads to enter the critical section simultaneously.

The drawback of the basic implementation is that since the C++ standard does not guarantee that the order of waking up from `notify` matches the order of `wait` calls (and in practice, such order is not observed), some threads may potentially starve or wait indefinitely to exit `enter`.

To avoid this, my semaphore guarantees that the order of `enter` calls matches the order of exiting them (and in terms of testing, matches the order of calls to the passed `callback`).

#### Rus

Здесь представлена усовершенствованная реализация одного из базовых примитивов синхронизации --- семафора. В отличие от мьютекса, семафор позволяет не одному, а сразу $`n`$ потокам входить в критическую секцию.

У базовой реализации есть недостаток --- поскольку стандарт C++ не гарантирует, что порядок пробуждения от `notify` совпадает с порядком вызовов `wait` (и на практике такой порядок действительно не наблюдается), то какие-то потоки потенциально могут голодать или же вообще ждать выхода из `enter` бесконечно долго.

Чтобы этого избежать, мой семафор гарантирует, что порядок вызова `enter` совпадает с порядком выхода из него (а в терминах тестирования с порядком вызовов передаваемого `callback`).