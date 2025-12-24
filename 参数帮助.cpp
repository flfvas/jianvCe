//============================================================================
// 参数列表
//============================================================================

// 3. 设置过滤器函数参数
interception_set_filter(
    // 参数 1 (Chuangjian_Shaangxiawen_0o0):
    // 传入之前创建的上下文句柄，指定这条过滤规则属于哪个通信实例。
    Chuangjian_Shaangxiawen_0o0,

    // 参数 2 (interception_is_keyboard):
    // 这是一个谓词函数，驱动会遍历所有硬件设备，只有符合“键盘”特征的设备才会被选中。
    interception_is_keyboard,

    // 参数 3 (INTERCEPTION_FILTER_KEY_ALL):
    // 过滤掩码。设置拦截该设备的所有按键状态（包括按下、弹起及扩展码），
    // 确保输入流的完整性，防止按键由于拦截不全而“粘连”。
    INTERCEPTION_FILTER_KEY_ALL
);



// 4. 接收数据循环中的参数处理
while (interception_receive
(
    // (Chuangjian_Shaangxiawen_0o0): 指定从哪个上下文读取数据。
    Chuangjian_Shaangxiawen_0o0,
    // (Shebei_0o0 = interception_wait(...)):
    //   - interception_wait(...) 的参数是上下文。
    //   - 它会阻塞并等待硬件产生信号，然后将产生信号的设备 ID 赋值给 Shebei_0o0。
    Shebei_0o0 = interception_wait(Chuangjian_Shaangxiawen_0o0),
    // (&Shuuru_Shuju_0o0): 传入结构体的地址，用于存储从驱动层抓取到的原始数据包内容。
    &Shuuru_Shuju_0o0,
    // (1): 本次接收的最大数据量。1 代表每次只处理一个按键动作，保证处理的实时性。
    1) > 0
)


// 5. 发送数据函数参数
interception_send
(
    // 参数 1 (Chuangjian_Shaangxiawen_0o0): 指明通过哪个上下文将数据写回内核。
    Chuangjian_Shaangxiawen_0o0,

    // 参数 2 (Shebei_0o0):
    // 关键参数。指明该数据包应该发往哪个物理设备 ID。
    // 这样系统就会认为这个“U”是从原来的物理键盘上按出来的。
    Shebei_0o0,

    // 参数 3 (&Shuuru_Shuju_0o0): 指向修改后的数据包地址。
    &Shuuru_Shuju_0o0,

    // 参数 4 (1): 本次发送的数据包数量。
    1
);


// 6. 销毁上下文参数
// (Chuangjian_Shaangxiawen_0o0): 传入句柄，通知驱动清理内存并断开通信。
interception_destroy_context
(
    Chuangjian_Shaangxiawen_0o0
);


