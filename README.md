# QChildProcessWidget

Виджет, отображающий окно дочернего процесса (например, браузера или, может, графического редактора).

Этот виджет меняет позицию, размер, а также видимость окна другого процесса, который сам при создании запускает.
На время перемещения или изменения размеров данного виджета, делается скриншот от окна дочернего процесса и отображается этот скриншот. По завершении изменения геометрии виджета окно дочернего виджета снова отображается уже с новой геометрией, поверх нашего окна.

## Ограничения:

### Первое

Как определить, какое окно нужно захватить: запускаемое окно должно иметь уникальный (и заранее известный нам!) заголовок.

Альтернативное решение: дочерний процесс при старте должен писать "window id: 0x12345678" первой строкой сразу после запуска. Получение id окна таким образом на данный момент **НЕ РЕАЛИЗОВАНО**.

### Второе

Работает корректно только когда один монитор

## License

&copy; Boris Vasilyev
[Борис Васильев](https://github.com/1024sparrow)

Licensed under the Apache License, Version 2.0 (the “License”); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
