#include <QCoreApplication> // 引入 Qt 核心应用类，用于管理事件循环
#include <QDebug>           // 引入 Qt 调试输出类，用于在控制台打印信息
#include "interception.h"   // 引入 Interception 驱动的头文件，这是操作内核输入流的关键

// 定义扫描码 (Scan Code)
// 扫描码是键盘硬件发送给计算机的原始数据，不依赖于操作系统语言设置
#define SCANCODE_M 0x32 // M 键的物理扫描码
#define SCANCODE_U 0x16 // U 键的物理扫描码

int main(int argc, char *argv[])
{
    // 程序(Chengxu) -> Chengxu_0o0
    QCoreApplication Chengxu_0o0(argc, argv);

    // Chuangjian(创建)_Shaangxiawen(上下文) -> Chuangjian_Shaangxiawen_0o0
    // 使用驱动接口创建一个通信句柄（Context），这是后续所有拦截操作的基础
    InterceptionContext Chuangjian_Shaangxiawen_0o0 = interception_create_context();

    // 检查驱动环境是否正常
    if (!Chuangjian_Shaangxiawen_0o0) {
        // 如果返回为空，通常是因为没有安装驱动或没有以管理员权限运行程序
        qCritical() << "错误: 无法建立上下文，请检查驱动是否以管理员权限运行！";
        return -1;
    }

    // 设置过滤器：告诉驱动我们需要拦截哪些类型的信号
    // 这里设置为拦截键盘设备（interception_is_keyboard）的所有按键动作（ALL）
    // 拦截 ALL 包含按下和弹起，这是防止按键被“粘住”的关键
    interception_set_filter(Chuangjian_Shaangxiawen_0o0,
                            interception_is_keyboard,
                            INTERCEPTION_FILTER_KEY_ALL);

    qDebug() << "映射程序已启动: [M] -> [U]";
    qDebug() << "提示: 请保持此窗口运行。按下 Ctrl+C 可停止映射。";

    // Shebei(设备) -> Shebei_0o0
    // 用于存储当前产生按键动作的物理设备 ID，Interception 支持识别多个键鼠
    InterceptionDevice Shebei_0o0;

    // Shuuru(输入)_Shuju(数据) -> Shuuru_Shuju_0o0
    // 定义一个通用的输入结构体，用于接收从内核驱动中截获的原始数据包
    InterceptionStroke Shuuru_Shuju_0o0;

    // 进入无限循环，持续接收并处理驱动层抛出的按键流
    // interception_wait 会阻塞线程，直到硬件产生输入动作
    while (interception_receive(Chuangjian_Shaangxiawen_0o0,
                                Shebei_0o0 = interception_wait(Chuangjian_Shaangxiawen_0o0),
                                &Shuuru_Shuju_0o0,
                                1) > 0)
    {
        // Anjian(按键)_Shuju(数据) -> Anjian_Shuju_0o0
        // 将通用的 stroke 结构体强制转换为键盘专用的结构体，以便访问 .code 属性
        InterceptionKeyStroke &Anjian_Shuju_0o0 = *(InterceptionKeyStroke *)&Shuuru_Shuju_0o0;

        // 逻辑篡改：检查当前拦截到的扫描码是否为 M 键
        // 这一步发生在内核驱动队列中，对应用层游戏是完全透明的
        if (Anjian_Shuju_0o0.code == SCANCODE_M) {
            // 将硬件原始的 M 扫描码修改为 U 的扫描码
            // 系统和游戏接下来会收到一个“硬件触发”的 U 键信号
            Anjian_Shuju_0o0.code = SCANCODE_U;
        }

        // 核心步骤：将处理完（修改后或原始）的数据包发回系统输入队列
        // 如果不执行这一步，所有的键盘输入都会被驱动“吞掉”，导致电脑无法输入
        interception_send(Chuangjian_Shaangxiawen_0o0, Shebei_0o0, &Shuuru_Shuju_0o0, 1);
    }

    // 程序退出前销毁句柄，释放驱动资源
    interception_destroy_context(Chuangjian_Shaangxiawen_0o0);
    return 0;
}
