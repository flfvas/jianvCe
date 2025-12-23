#include <QCoreApplication>
#include <QDebug>
#include "interception.h"

// 定义扫描码
#define SCANCODE_M 0x32
#define SCANCODE_U 0x16

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    InterceptionContext context = interception_create_context();

    if (!context) {
        qCritical() << "错误: 无法建立上下文，请检查驱动是否以管理员权限运行！";
        return -1;
    }

    // 设置过滤器：只拦截键盘的按下(DOWN)和弹起(UP)动作
    // 必须拦截 UP 动作，否则按键会一直处于按下状态
    interception_set_filter(context, interception_is_keyboard,
                            INTERCEPTION_FILTER_KEY_ALL);

    qDebug() << "映射程序已启动: [M] -> [U]";
    qDebug() << "提示: 请保持此窗口运行。按下 Ctrl+C 可停止映射。";

    InterceptionDevice device;
    InterceptionStroke stroke;

    // 持续接收并处理按键流
    while (interception_receive(context, device = interception_wait(context), &stroke, 1) > 0)
    {
        // 将通用的 stroke 转换为键盘专用的结构体
        InterceptionKeyStroke &keyStroke = *(InterceptionKeyStroke *)&stroke;

        // 如果按下的键是 M (0x32)
        if (keyStroke.code == SCANCODE_M) {
            // 修改扫描码为 U (0x16)
            keyStroke.code = SCANCODE_U;
        }

        // 将修改后或原始的按键发送回系统
        // 这一步是全局有效的关键
        interception_send(context, device, &stroke, 1);
    }

    interception_destroy_context(context);
    return 0;
}
