实验一: 操作系统初步

# 系统调用实验
  **系统调用：**
Linux内核中设置了一组用于实现各种系统功能的子程序，称为系统调用。用户可以通过系统调用命令在自己的应用程序中调用它们。从某种角度来看，系统调用和普通的函数调用非常相似。区别仅仅在于，系统调用由操作系统核心提供，运行于核心态；而普通的函数调用由函数库或用户自己提供，运行于用户态。
 **系统调用的用法**
进程是不能访问内核的。它不能访问内核所占内存空间也不能调用内核函数。系统调用是这些规则的一个例外。其原理是进程先用适当的值填充寄存器，然后调用一个特殊的指令，这个指令会跳到一个事先定义的内核中的一个位置。在Intel CPU中，这个由中断0x80实现。进程可以跳转到的内核位置叫做sysem_call。这个过程检查系统调用号，这个号码告诉内核进程请求哪种服务。然后，它查看系统调用表(sys_call_table)找到所调用的内核函数入口地址。接着，就调用函数，等返回后进行系统检查，最后返回到进程。
## 实验内容
 1. **运行用API接口函数getpid()直接调用方式调用Linux操作系统的系统调用getpid的程序。**
 编写c程序，功能为利用getpid（）函数获得进程号同时打印输出进程号。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315212304250.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
 利用gcc命令编译运行，得到进程号。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315212353128.png)
 根据查询可得到getpid的系统调用号是39。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315212527615.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
 在 `/usr/include/unistd.h`中可以找到该函数
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/2019031521255526.png)
 4. **运行汇编中断调用方式调用Linux操作系统的系统调用getpid的程序。**
编写c程序，在c语言中内嵌汇编代码实现系统调用。此处系统调用是一个软中断，既然是中断那么一般就具有中断号和中断处理程序两个属性，Linux使用0x80号中断作为系统调用的入口，而中断处理程序的地址放在中断向量表里，通过代码可知getpid的中断向量号为14。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315212745385.png?x-ossprocess=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
利用gcc命令编译运行，得到进程号。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315212813639.png)
分别运行程序1.1.1和1.1.2可获得两个相邻的进程号。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315212835999.png)
 5. **完成习题1.13：命令：`printf(“Hello World!\n”)` 分别用相应的linux系统调用的C函数形式和汇编代码两种形式来实现上述命令。**
 1）以C函数形式实现printf
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315213306158.png)
 编译运行1.1.3.c，可见打印输出“Hello World”
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315213326704.png)
 2） 以汇编代码形式实现printf
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/2019031521340235.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
 编写好上述代码，然后将其保存为printf.asm，先使用NASM编译 hello.asm程序: `nasm –f  elf64  printf.asm`。之后使用 `ld –s –o  printf  printf.o` 指令连接程序，最后运行程序。
可见程序输出：“Hello World”
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315214304915.png)
 6. **画出系统调用实现的流程图**
 Pintos是80*86架构的简单操作系统框架。支持内核线程，加载和运行用户程序以及文件系统。
 下载Pintos解压缩后可以看到src下的目录：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315233807850.PNG?xossprocess=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
在lib/user/syscall.h中可以看到定义了20个系统调用函数
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315234513238.PNG?xossprocess=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
在syscall.c中可见有四个函数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315235120746.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
系统调用流程图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315235655515.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70) 
## 问题总结
 1. **系统调用号的查询**
在Linux中，每个系统调用被赋予一个系统调用号。这样，通过这个独一无二的号就可以关联系统调用。当用户空间的进程执行一个系统调用的时候，这个系统调用号就被用来指明到底是要执行哪个系统调用。
在linux查看32位的系统调用号：`cat /usr/include/asm/unistd_32.h` 
在linux查看64位的系统调用号：`cat /usr/include/asm/unistd_64.h`
 8. **linux下的系统调用**
系统调用可以通过syscall()函数发起，或者调用每个对应的一个C函数，这些函数定义在<syscall.h> 或者<unistd.h> 头文件中。应用程序通过应用编程接口(API)而不是直接通过系统调用来编程。因为应用程序使用的这种编程接口实际上并不需要和内核提供的系统调用一一对应。一个API定义了一组应用程序使用的编程接口。它们可以实现成一个系统调用，也可以通过调用多个系统调用来实现，而完全不使用任何系统调用也不存在问题。
用户空间的程序无法直接执行内核代码。它们不能直接调用内核空间中的函数，因为内核驻留在受保护的地址空间上。如果进程可以直接在内核的地址空间上读写的话，系统安全就会失去控制。所以，通知内核的机制是靠软件中断实现的。首先，用户程序为系统调用设置参数。其中一个参数是系统调用编号，如本实验中的getpid为39。参数设置完成后，程序执行“系统调用”指令。x86系统上的软中断由int产生。这个指令会导致一个异常：产生一个事件，这个事件会致使处理器切换到内核态并跳转到一个新的地址，并开始执行那里的异常处理程序。此时的异常处理程序实际上就是系统调用处理程序。
Linux下的系统调用主要通过以下几个步骤：
（1）将你的系统调用号放进EAX中。
（2）设置系统调用参数，并且依次将参数放进EBX、ECX、EDX、ESX、EDI和EBP。
（3）调用相关中断（对应Linux来说是 80h）。
（4）最后的调用结果会返回到EAX中保存。
# 并发实验
## 实验内容
 1. **编译运行cpu.c，观察输出结果，说明程序功能。**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315220115923.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315220138427.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
编译并运行cpu.c文件，可见连续输出指定字符。该程序功能为每隔一秒输出一次命令行参数，若没有正确接受参数则提示用户正确输入参数。仅运行一个cpu程序时：循环输出“A”
2. **同时运行多个cpu程序**
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315220352413.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
同时运行四个cpu程序时，可见同时有四个进程：30879、30880、30881、30882，在运行时可以看到是四个程序交替运行，四个进程轮流运行，顺序并无一定的规律。同时进行任务A、B、C、D，CPU会轮流地分配给A、B、C、D使用，只是之间的时间间隔太小，用户不能感觉出来，就觉得是多个任务同时进行一样，这就是并发性。在本实验中，通过sleep函数加长时间间隔。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315220432571.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
通过命令行去随机化操作后，再次运行程序，可以到的以下结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315220640231.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
程序均已“BACD”的顺序输出，可见此时已经不再是之前的随机算法。
## 问题总结
1. **gcc编译时 –Wall 的含义**
 -Wall选项意思是编译后显示所有警告；-w的意思是关闭编译时的警告，也就是编译后不显示任何warning，因为有时在编译之后编译器会显示一些例如数据转换之类的警告，这些警告是我们平时可以忽略的。-W选项类似-Wall，会显示警告，但是只显示编译器认为会出现错误的警告。
2. **操作系统的并发和并行**
并行性和并发性是既相似又有区别的两个概念。并行性是指两个或多个事件在同一时刻发生。而并发性是指两个或多个事件在同一时间间隔发生。在多道程序环境下，并发性是指在一段时间内宏观上有多个程序在同时运行，例如本例中A、B、C、D四个cpu程序在同时运行，但在单处理机系统中，每一时刻却仅能有一道程序执行，故微观上这些程序只能是分时地交替执行。
# 内存分配实验
## 实验内容
1. **编译运行mem.c，观察输出结果，说明程序功能。**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315220959631.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315221029231.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
编译并运行mem.c文件，可见运行`./mem`时程序输出运行进程的进程号以及所指向地址的初始位置，并循环使下一指针所指向的内容加1，以检验是否共享同一块物理内存区域。
仅运行一个mem.c程序时：初始状态p = 0x1ce2010  *p = 0，可见该进程循环使得p指向的值逐次累加1。
2. **运行两个mem程序**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315221142295.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
同时运行两个mem.c程序时，可以看到，此时拥有两个进程：31314和31315。其中进程31315分配的内存地址为：0x1c3e010，进程31314分配的内存地址为0x2200010，所以两个分别运行的程序飞配的内存地址不相同。
之后两个进程开始进入循环部分，可见进程31314每循环一次，指针p所指向的内容加1，此后进程31315每循环一次，指针p在原有的基础上所指向内容加1，并未受到31314进程的影响。所以可知两个分别运行的程序不共享同一块物理内存区域。

## 问题总结
1.  **C语言中***p=***p+1的含义，以及各类指针操作辨析**
*p=*p+1的效果：指针不移动，但指针所指的数据有+1效果。
*p++的效果：指针向下移动一个单位（对于int变量的数组，指针移动了4字节），指针内的内容并未变化。
*(p+1)的效果：p指向的内存地址的下一个地址的数据
3. **输出地址的方法**
%p格式符会输出指针本身的值，也就是指针指向的地址值。该输出为16进制形式，具体输出值取决于指针指向的实际地址值。%p一般仅用于printf及同类函数中。形式为`printf("%p", varp);` 其中后续参数varp为某一个指针变量。

# 共享实验
## 实验内容
1. **编译运行thread.c，观察输出结果，说明程序功能。**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315222944716.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
在Linux系统下，与线程相关的函数都定义在pthread.h头文件中。
创建线程函数是pthread_create函数。线程等待函数pthread_join函数，调用该函数的线程将挂起等待,直到线程终止。thread.c程序功能是将输入的命令行参数传入，创建两个线程，并在指定线程运行worker函数完成计数。由此判断全局变量在不同的线程当中访问全局变量是否是共享的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315223023643.png)
执行测试，发现得到的统计值为输入值的两倍。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315223059831.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mzc1Mjk1Mw==,size_16,color_FFFFFF,t_70)
多次测试发现仍然符合该规律，final value的值总是为输入参数的二倍。在这个函数中，worker函数是两个线程共有的运行函数，全局变量loops，counter是两个线程共享的，loops值等于输入的参数值，而两个线程执行时，由结果可见counter值是可累加的，所以可知全局变量在多个线程中是共享的
虽然线程共享全局变量相对于进程通信会给线程通信带来巨大的方便，但是不做控制的进行访问全局变量也是致命的，带来巨大程序bug，并且难以发现。当多个线程同时对一个全局变量操作，会出现资源竞争问题，从而导致数据结不正确，即遇到线性安全问题。
## 问题总结
**1. 进程与线程的区别**
进程：程序的一个动态运行实例，承担分配系统资源的实例。（Linux实现进程的主要目的是资源独占）
线程：在进程的内部运行（进程的地址空间）运行的一个分支，也是调度的基本单位。（Linux实现线程的主要目的是资源共享）。线程所有的资源由进程提供。
**2. gcc中-pthread和-lpthread的区别**
编译选项中指定 -pthread 会附加一个宏定义 -D_REENTRANT，该宏会导致libc头文件选择那些thread-safe的实现；链接选项中指定 -pthread 则同 -lpthread 一样，只表示链接 POSIX thread 库。由于 libc 用于适应 thread-safe 的宏定义可能变化，因此在编译和链接时都使用 -pthread 选项而不是传统的 -lpthread 能够保持向后兼容，并提高命令行的一致性。

# 实验总结
通过本次实验，我对于操作系统的系统调用，并发性，内存分配，共享问题等有了进一步的理解，同时也对于linux系统掌握了基本的操作。在这四个不同的实验过程中，出现了很多知识上的短缺和疑惑，通过查询学习即时的填补了知识空缺。在完成实验后更深的理解了课堂上所讲的概念，尤其直观的看到关于并发的交替进行和内存分配等比较抽象的问题，也加深了记忆。此外由于之前欠缺了汇编方面的知识，在本次实验中通过了解学习我也掌握了一些关于汇编知识的运用。希望之后能够继续加强实验动手能力，加油~
