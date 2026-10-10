校园食堂订餐管理系统
├── main函数                // 程序入口，初始化数据、登录逻辑
├── 结构体定义
│   ├── Dish               // 菜品
│   ├── User               // 学生用户
│   ├── Manager            // 管理员
│   ├── ClassInfo          // 班级信息
│   ├── BuyItem            // 订单内单个菜品项
│   ├── Order              // 订单
│   ├── Review             // 菜品评价
│   ├── Complaint          // 投诉单
│   └── StockRecord        // 库存变动记录
├── 基础工具函数
│   ├── intToStr           // int转字符串，生成订单号
│   ├── findDish/findUser  // 查找菜品、学生
│   ├── loadAll/saveAll    // 文件加载、文件保存全部数据
│   └── updateClassCount   // 自动统计班级学生人数
├── 学生端功能函数
├── 管理员端功能函数
├── 各级菜单函数
└── 持久化txt数据文件
