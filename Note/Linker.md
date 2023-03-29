# A simple linker script
- `ENTRY(symbol)`: 設定某個symbol為程式執行的第一個指令起始點
- `OUTPUT_FORMAT`: output file format, e.g. binary(.bin)
- `loadable`: 執行時該section是否需要被載入到記憶體
- `allocatable`: 先保留記憶體的一塊空間讓程式執行時使用，如.bss


Reference:
- [GNU linker script](https://www.eecs.umich.edu/courses/eecs373/readings/Linker.pdf)
- [Linker Script初探 - GNU Linker Ld手冊略讀](http://wen00072.github.io/blog/2014/03/14/study-on-the-linker-script/#assign)