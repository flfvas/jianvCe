#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include "interception.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "--- Interception 驱动检测程序 ---";

    // 1. 创建上下文
    InterceptionContext context = interception_create_context();

    if (!context) {
        qCritical() << "错误: 无法创建上下文。驱动可能未安装或权限不足！";
        return -1;
    }

    qDebug() << "成功: 已建立驱动上下文。";

    // 2. 设置过滤器：监控所有键盘的按下动作
    interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN);

    qDebug() << "正在等待键盘输入 (请按下任意键)...";
    qDebug() << "提示: 如果驱动正常且识别了 PS/2，你将看到设备 ID。";

    InterceptionDevice device;
    InterceptionStroke stroke;

    // 3. 循环等待输入
    // 注意：如果在 identify.exe 中显示为 (0)，这里通常会阻塞或返回 device=0
    while (interception_receive(context, device = interception_wait(context), &stroke, 1) > 0)
    {
        qDebug() << "收到输入! 设备 ID (Device Index):" << device;

        if (device == 0) {
            qWarning() << "警告: 收到信号但设备 ID 为 0。这通常意味着 PS/2 驱动挂载失败。";
        } else {
            qDebug() << "成功识别到有效硬件设备！";
        }

        // 必须将按键发回系统，否则你的键盘会被锁死
        interception_send(context, device, &stroke, 1);

        // 检测到一个按键后退出，防止测试时键盘失灵
        break;
    }

    qDebug() << "测试结束，释放资源。";
    interception_destroy_context(context);

    return 0;
}
