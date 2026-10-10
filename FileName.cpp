#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
using namespace std;

// ===================== 全局常量定义 =====================
const int MAX_DISH = 50;        // 菜品最大数量
const int MAX_USER = 500;       // 用户（学生/管理员）最大数量
const int MAX_ORDER = 1000;     // 订单最大数量
const int MAX_CLASS = 50;       // 班级最大数量
const int MAX_REVIEW = 1000;    // 菜品评价最大数量
const int MAX_COMPLAINT = 500;  // 投诉记录最大数量
const int MAX_STOCK = 2000;     // 库存变动记录最大数量
const int LOW_STOCK = 10;       // 低库存预警阈值
const int MAX_ORDER_ITEM = 10;  // 单个订单最多可选菜品种类

// ===================== 数据结构体定义 =====================
// 菜品信息结构体
struct Dish {
    int id;                 // 菜品编号
    string name;            // 菜品名称
    double price;           // 菜品单价
    int stock;              // 当前库存份数
    double totalRating;     // 累计评分总分
    int ratingCount;        // 累计评价人数
};

// 班级信息结构体
struct ClassInfo {
    string className;       // 班级名称
    string major;           // 所属专业
    int studentCount;       // 班级学生总数
};

// 学生用户结构体
struct User {
    string id;              // 学号
    string name;            // 姓名
    string pwd;             // 登录密码
    int grade;              // 年级
    string major;           // 专业
    string className;       // 所在班级
    string birth;           // 出生日期
    string number;          // 联系电话
    double money;           // 校园卡账户余额
};

// 管理员/食堂员工结构体
struct Manager {
    string id;              // 工号
    string name;            // 姓名
    string pwd;             // 登录密码
};

// 订单内单个菜品项
struct BuyItem {
    int dishId;             // 菜品ID
    int num;                // 购买份数
};

// 订单结构体
struct Order {
    string orderId;         // 订单编号
    string userId;          // 下单学生学号
    BuyItem list[MAX_ORDER_ITEM]; // 订单菜品列表
    int itemCnt;            // 订单内菜品种类数
    double total;           // 订单总金额
    int status;             // 订单状态：0-未支付 1-已支付待取餐 2-已取餐
};

// 菜品评价结构体
struct Review {
    string orderId;         // 关联订单号
    string userId;          // 评价人学号
    int dishId;             // 被评价菜品ID
    int star;               // 星级评分 1~5
    string comment;         // 文字评价内容
};

// 投诉建议结构体
struct Complaint {
    int id;                 // 投诉编号
    string userId;          // 投诉人学号
    string type;            // 投诉类型
    string content;         // 投诉内容
    string reply;           // 管理员回复
    int status;             // 处理状态：0-待处理 1-已处理
};

// 库存变动记录结构体
struct StockRecord {
    int id;                 // 记录编号
    int dishId;             // 菜品ID
    int quantity;           // 变动数量
    string opType;          // 操作类型：采购/消耗
    string date;            // 操作日期
    string operatorName;    // 操作人姓名
};

// ===================== 全局数据数组与计数器 =====================
Manager mgrList[MAX_USER];      // 管理员数组
int mgrCnt = 0;                 // 管理员总数
Dish dishList[MAX_DISH];        // 菜品数组
int dishCnt = 0;                // 菜品总数
User userList[MAX_USER];        // 学生数组
int userCnt = 0;                // 学生总数
Order orderList[MAX_ORDER];     // 订单数组
int orderCnt = 0;               // 订单总数
ClassInfo classList[MAX_CLASS]; // 班级数组
int classCnt = 0;               // 班级总数
Review reviewList[MAX_REVIEW];  // 评价数组
int reviewCnt = 0;              // 评价总数
Complaint complaintList[MAX_COMPLAINT]; // 投诉数组
int complaintCnt = 0;           // 投诉总数
StockRecord stockList[MAX_STOCK]; // 库存记录数组
int stockCnt = 0;               // 库存记录总数

int loginIdx = -1;              // 当前登录用户的数组下标，-1表示未登录
bool isManager = false;         // 当前登录角色标记：true管理员 false学生
string currentDate = "2026-07-21"; // 系统当前日期

// ===================== 工具函数 =====================
// 整数转字符串，用于生成订单号
string intToStr(int num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

// 根据菜品ID查找，返回数组下标；找不到返回-1
int findDish(int id) {
    for (int i = 0; i < dishCnt; i++) {
        if (dishList[i].id == id) return i;
    }
    return -1;
}

// 根据学号查找学生，返回数组下标；找不到返回-1
int findUser(string id) {
    for (int i = 0; i < userCnt; i++) {
        if (userList[i].id == id) return i;
    }
    return -1;
}

// 根据班级名查找班级，返回数组下标；找不到返回-1
int findClass(string name) {
    for (int i = 0; i < classCnt; i++) {
        if (classList[i].className == name) return i;
    }
    return -1;
}

// 获取当前登录操作人的姓名
string getOperator() {
    if (isManager) {
        return mgrList[loginIdx].name;
    }
    else {
        return userList[loginIdx].name;
    }
}

// 更新所有班级的学生人数统计
void updateClassCount() {
    for (int i = 0; i < classCnt; i++) {
        int cnt = 0;
        for (int j = 0; j < userCnt; j++) {
            if (userList[j].className == classList[i].className) {
                cnt++;
            }
        }
        classList[i].studentCount = cnt;
    }
}

// ===================== 文件持久化函数 =====================
// 将所有数据保存到本地TXT文件
void saveAll() {
    ofstream f;

    // 保存菜品数据
    f.open("data_dish.txt");
    f << dishCnt << endl;
    for (int i = 0; i < dishCnt; i++) {
        f << dishList[i].id << " "
            << dishList[i].name << " "
            << dishList[i].price << " "
            << dishList[i].stock << " "
            << dishList[i].totalRating << " "
            << dishList[i].ratingCount << endl;
    }
    f.close();

    // 保存学生数据
    f.open("data_user.txt");
    f << userCnt << endl;
    for (int i = 0; i < userCnt; i++) {
        f << userList[i].id << " "
            << userList[i].name << " "
            << userList[i].pwd << " "
            << userList[i].grade << " "
            << userList[i].major << " "
            << userList[i].className << " "
            << userList[i].birth << " "
            << userList[i].number << " "
            << userList[i].money << endl;
    }
    f.close();

    // 保存管理员数据
    f.open("data_manager.txt");
    f << mgrCnt << endl;
    for (int i = 0; i < mgrCnt; i++) {
        f << mgrList[i].id << " "
            << mgrList[i].name << " "
            << mgrList[i].pwd << endl;
    }
    f.close();

    // 保存订单数据（含订单内菜品明细）
    f.open("data_order.txt");
    f << orderCnt << endl;
    for (int i = 0; i < orderCnt; i++) {
        f << orderList[i].orderId << " "
            << orderList[i].userId << " "
            << orderList[i].itemCnt << " "
            << orderList[i].total << " "
            << orderList[i].status << endl;
        for (int j = 0; j < orderList[i].itemCnt; j++) {
            f << orderList[i].list[j].dishId << " "
                << orderList[i].list[j].num << endl;
        }
    }
    f.close();

    // 保存班级数据
    f.open("data_class.txt");
    f << classCnt << endl;
    for (int i = 0; i < classCnt; i++) {
        f << classList[i].className << " "
            << classList[i].major << " "
            << classList[i].studentCount << endl;
    }
    f.close();

    // 保存评价数据
    f.open("data_review.txt");
    f << reviewCnt << endl;
    for (int i = 0; i < reviewCnt; i++) {
        f << reviewList[i].orderId << " "
            << reviewList[i].userId << " "
            << reviewList[i].dishId << " "
            << reviewList[i].star << " "
            << reviewList[i].comment << endl;
    }
    f.close();

    // 保存投诉数据
    f.open("data_complaint.txt");
    f << complaintCnt << endl;
    for (int i = 0; i < complaintCnt; i++) {
        f << complaintList[i].id << " "
            << complaintList[i].userId << " "
            << complaintList[i].type << " "
            << complaintList[i].content << " "
            << complaintList[i].reply << " "
            << complaintList[i].status << endl;
    }
    f.close();

    // 保存库存变动记录
    f.open("data_stock.txt");
    f << stockCnt << endl;
    for (int i = 0; i < stockCnt; i++) {
        f << stockList[i].id << " "
            << stockList[i].dishId << " "
            << stockList[i].opType << " "
            << stockList[i].quantity << " "
            << stockList[i].date << " "
            << stockList[i].operatorName << endl;
    }
    f.close();
}

// 从本地TXT文件加载所有数据
void loadAll() {
    ifstream f;

    // 加载菜品
    f.open("data_dish.txt");
    if (f.is_open()) {
        f >> dishCnt;
        // 读取失败或数值越界则重置为0
        if (f.fail() || dishCnt < 0 || dishCnt > MAX_DISH) dishCnt = 0;
        for (int i = 0; i < dishCnt; i++) {
            f >> dishList[i].id
                >> dishList[i].name
                >> dishList[i].price
                >> dishList[i].stock
                >> dishList[i].totalRating
                >> dishList[i].ratingCount;
            if (f.fail()) {
                dishCnt = i;
                break;
            }
        }
    }
    f.close();

    // 加载学生
    f.open("data_user.txt");
    if (f.is_open()) {
        f >> userCnt;
        if (f.fail() || userCnt < 0 || userCnt > MAX_USER) userCnt = 0;
        for (int i = 0; i < userCnt; i++) {
            f >> userList[i].id
                >> userList[i].name
                >> userList[i].pwd
                >> userList[i].grade
                >> userList[i].major
                >> userList[i].className
                >> userList[i].birth
                >> userList[i].number
                >> userList[i].money;
            if (f.fail()) {
                userCnt = i;
                break;
            }
        }
    }
    f.close();

    // 加载管理员
    f.open("data_manager.txt");
    if (f.is_open()) {
        f >> mgrCnt;
        if (f.fail() || mgrCnt < 0 || mgrCnt > MAX_USER) mgrCnt = 0;
        for (int i = 0; i < mgrCnt; i++) {
            f >> mgrList[i].id
                >> mgrList[i].name
                >> mgrList[i].pwd;
            if (f.fail()) {
                mgrCnt = i;
                break;
            }
        }
    }
    f.close();

    // 加载订单
    f.open("data_order.txt");
    if (f.is_open()) {
        f >> orderCnt;
        if (f.fail() || orderCnt < 0 || orderCnt > MAX_ORDER) orderCnt = 0;
        for (int i = 0; i < orderCnt; i++) {
            f >> orderList[i].orderId
                >> orderList[i].userId
                >> orderList[i].itemCnt
                >> orderList[i].total
                >> orderList[i].status;
            if (f.fail()) { orderCnt = i; break; }
            // 读取订单内的每一项菜品
            for (int j = 0; j < orderList[i].itemCnt; j++) {
                f >> orderList[i].list[j].dishId
                    >> orderList[i].list[j].num;
                if (f.fail()) break;
            }
        }
    }
    f.close();

    // 加载班级
    f.open("data_class.txt");
    if (f.is_open()) {
        f >> classCnt;
        if (f.fail() || classCnt < 0 || classCnt > MAX_CLASS) classCnt = 0;
        for (int i = 0; i < classCnt; i++) {
            f >> classList[i].className
                >> classList[i].major
                >> classList[i].studentCount;
            if (f.fail()) {
                classCnt = i;
                break;
            }
        }
    }
    f.close();

    // 加载评价
    f.open("data_review.txt");
    if (f.is_open()) {
        f >> reviewCnt;
        if (f.fail() || reviewCnt < 0 || reviewCnt > MAX_REVIEW) reviewCnt = 0;
        for (int i = 0; i < reviewCnt; i++) {
            f >> reviewList[i].orderId
                >> reviewList[i].userId
                >> reviewList[i].dishId
                >> reviewList[i].star
                >> reviewList[i].comment;
            if (f.fail()) {
                reviewCnt = i;
                break;
            }
        }
    }
    f.close();

    // 加载投诉
    f.open("data_complaint.txt");
    if (f.is_open()) {
        f >> complaintCnt;
        if (f.fail() || complaintCnt < 0 || complaintCnt > MAX_COMPLAINT) complaintCnt = 0;
        for (int i = 0; i < complaintCnt; i++) {
            f >> complaintList[i].id
                >> complaintList[i].userId
                >> complaintList[i].type
                >> complaintList[i].content
                >> complaintList[i].reply
                >> complaintList[i].status;
            if (f.fail()) {
                complaintCnt = i;
                break;
            }
        }
    }
    f.close();

    // 加载库存记录
    f.open("data_stock.txt");
    if (f.is_open()) {
        f >> stockCnt;
        if (f.fail() || stockCnt < 0 || stockCnt > MAX_STOCK) stockCnt = 0;
        for (int i = 0; i < stockCnt; i++) {
            f >> stockList[i].id
                >> stockList[i].dishId
                >> stockList[i].opType
                >> stockList[i].quantity
                >> stockList[i].date
                >> stockList[i].operatorName;
            if (f.fail()) {
                stockCnt = i;
                break;
            }
        }
    }
    f.close();
}

// 系统初始化：加载文件，无数据时写入默认测试数据
void init() {
    loadAll();
    // 初始化默认菜品
    if (dishCnt == 0) {
        dishList[dishCnt++] = { 1, "红烧肉饭", 15, 50, 0, 0 };
        dishList[dishCnt++] = { 2, "黄焖鸡", 16, 30, 0, 0 };
        dishList[dishCnt++] = { 3, "麻辣香锅", 25, 20, 0, 0 };
        dishList[dishCnt++] = { 4, "番茄炒蛋", 10, 40, 0, 0 };
        dishList[dishCnt++] = { 5, "宫保鸡丁", 14, 35, 0, 0 };
    }
    // 初始化默认班级
    if (classCnt == 0) {
        classList[classCnt++] = { "计科2301", "计算机科学与技术", 0 };
        classList[classCnt++] = { "软工2302", "软件工程", 0 };
        classList[classCnt++] = { "智科2301", "智能科学与技术", 0 };
    }
    // 初始化默认学生账号
    if (userCnt == 0) {
        userList[userCnt++] = { "stu001", "小明", "123", 7, "计算机科学与技术", "计科2301", "2000-01-01", "12345678901", 100.0 };
        userList[userCnt++] = { "stu002", "小红", "123", 7, "软件工程", "软工2302", "2001-03-15", "13900001111", 80.0 };
    }
    // 初始化默认管理员账号
    if (mgrCnt == 0) {
        mgrList[mgrCnt++] = { "sta001", "打饭阿姨", "123" };
        mgrList[mgrCnt++] = { "man001", "管理员", "123" };
    }
    updateClassCount(); // 更新班级人数统计
}

// ===================== 登录模块 =====================
// 登录验证：区分学生和管理员，成功返回数组下标，失败返回-1
int login() {
    string id, pwd;
    cout << "\n========== 登录 ==========\n";
    cout << "账号：";
    cin >> id;
    cout << "密码：";
    cin >> pwd;

    // 先匹配学生账号
    for (int i = 0; i < userCnt; i++) {
        if (userList[i].id == id && userList[i].pwd == pwd) {
            cout << "欢迎学生 " << userList[i].name << endl;
            isManager = false;
            return i;
        }
    }
    // 再匹配管理员账号
    for (int i = 0; i < mgrCnt; i++) {
        if (mgrList[i].id == id && mgrList[i].pwd == pwd) {
            cout << "欢迎员工 " << mgrList[i].name << endl;
            isManager = true;
            return i;
        }
    }
    cout << "账号或密码错误\n";
    return -1;
}

// ===================== 学生端功能函数 =====================
// 展示食堂菜品菜单，含价格、库存、评分、低库存预警
void showMenu() {
    cout << "\n========== 食堂菜单 ==========\n";
    cout << left << setw(6) << "ID"
        << setw(14) << "菜名"
        << setw(8) << "价格"
        << setw(8) << "库存"
        << setw(10) << "平均分" << endl;
    cout << "----------------------------------------\n";
    for (int i = 0; i < dishCnt; i++) {
        double avg = 0;
        if (dishList[i].ratingCount > 0) {
            avg = dishList[i].totalRating / dishList[i].ratingCount;
        }
        cout << left << setw(6) << dishList[i].id
            << setw(14) << dishList[i].name
            << setw(8) << dishList[i].price
            << setw(8) << dishList[i].stock;
        // 有评价则显示平均分，否则显示暂无
        if (dishList[i].ratingCount > 0) {
            cout << setw(10) << avg;
        }
        else {
            cout << setw(10) << "暂无";
        }
        // 库存低于阈值显示预警
        if (dishList[i].stock <= LOW_STOCK) {
            cout << " [库存不足!]";
        }
        cout << endl;
    }
}

// 学生点餐：选菜、校验库存余额、扣钱扣库存、生成订单
void studentOrder() {
    showMenu();
    Order ord;
    ord.orderId = "ORD" + intToStr(1000 + orderCnt); // 生成唯一订单号
    ord.userId = userList[loginIdx].id;
    ord.itemCnt = 0;
    ord.total = 0;
    ord.status = 0; // 初始未支付状态
    int dishId, num;

    // 循环选菜，输入0结束
    while (true) {
        if (ord.itemCnt >= MAX_ORDER_ITEM) {
            cout << "\n订单菜品已达上限(" << MAX_ORDER_ITEM << "种)\n";
            break;
        }
        cout << "\n输入菜品ID(0结束)：";
        cin >> dishId;
        // 输入非法字符处理
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入无效\n";
            continue;
        }
        if (dishId == 0) break;

        int idx = findDish(dishId);
        if (idx == -1) {
            cout << "菜品不存在\n";
            continue;
        }
        cout << "数量：";
        cin >> num;
        // 校验数量合法性与库存
        if (cin.fail() || num <= 0 || num > dishList[idx].stock) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "数量无效或库存不足\n";
            continue;
        }
        // 加入订单
        ord.list[ord.itemCnt].dishId = dishId;
        ord.list[ord.itemCnt].num = num;
        ord.itemCnt++;
        ord.total += dishList[idx].price * num;
        cout << "已加入\n";
    }

    if (ord.itemCnt == 0) {
        cout << "未选菜\n";
        return;
    }

    cout << "总价：" << ord.total << " 余额：" << userList[loginIdx].money << "\n";
    cout << "确认支付？(1/0)：";
    int confirm;
    cin >> confirm;
    if (confirm != 1) {
        cout << "已取消\n";
        return;
    }
    // 余额校验
    if (userList[loginIdx].money < ord.total) {
        cout << "余额不足\n";
        return;
    }

    // 扣减账户余额
    userList[loginIdx].money -= ord.total;
    // 扣减菜品库存，记录库存消耗流水
    for (int i = 0; i < ord.itemCnt; i++) {
        int idx = findDish(ord.list[i].dishId);
        if (idx != -1) {
            dishList[idx].stock -= ord.list[i].num;
            // 写入库存变动记录
            if (stockCnt < MAX_STOCK) {
                stockList[stockCnt].id = stockCnt + 1;
                stockList[stockCnt].dishId = ord.list[i].dishId;
                stockList[stockCnt].quantity = ord.list[i].num;
                stockList[stockCnt].opType = "消耗";
                stockList[stockCnt].date = currentDate;
                stockList[stockCnt].operatorName = userList[loginIdx].name;
                stockCnt++;
            }
            // 低库存提示
            if (dishList[idx].stock <= LOW_STOCK) {
                cout << "【预警】" << dishList[idx].name
                    << " 库存仅剩 " << dishList[idx].stock << " 份\n";
            }
        }
    }
    ord.status = 1; // 更新为已支付待取餐
    orderList[orderCnt++] = ord;
    saveAll();
    cout << "支付成功！订单号：" << ord.orderId << endl;
}

// 学生账户充值
void studentRecharge() {
    double money;
    cout << "\n当前余额：" << userList[loginIdx].money << "\n";
    cout << "充值金额：";
    cin >> money;
    if (cin.fail() || money <= 0) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "金额必须大于0\n";
        return;
    }
    userList[loginIdx].money += money;
    saveAll();
    cout << "充值成功，新余额：" << userList[loginIdx].money << endl;
}

// 查看当前学生的所有订单
void myOrders() {
    cout << "\n========== 我的订单 ==========\n";
    bool found = false;
    string status[] = { "未支付", "已支付待取餐", "已取餐" };
    for (int i = 0; i < orderCnt; i++) {
        if (orderList[i].userId == userList[loginIdx].id) {
            found = true;
            cout << "订单号：" << orderList[i].orderId
                << " 金额：" << orderList[i].total
                << " 状态：" << status[orderList[i].status] << "\n";
            cout << "菜品：";
            for (int j = 0; j < orderList[i].itemCnt; j++) {
                int idx = findDish(orderList[i].list[j].dishId);
                if (idx != -1) {
                    cout << dishList[idx].name
                        << "x" << orderList[i].list[j].num << " ";
                }
            }
            cout << endl;
        }
    }
    if (!found) cout << "暂无订单\n";
}

// 学生对订单菜品提交评价
void addReview() {
    cout << "\n========== 评价 ==========\n";
    myOrders(); // 先展示自己的订单
    string oid;
    cout << "输入订单号：";
    cin >> oid;
    int oi = -1;
    // 查找属于自己的订单
    for (int i = 0; i < orderCnt; i++) {
        if (orderList[i].orderId == oid && orderList[i].userId == userList[loginIdx].id) {
            oi = i;
            break;
        }
    }
    if (oi == -1) {
        cout << "订单不存在\n";
        return;
    }

    int dishId, star;
    string comment;
    cout << "菜品ID：";
    cin >> dishId;
    cout << "评分(1-5)：";
    cin >> star;
    if (cin.fail() || star < 1 || star > 5) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "评分无效\n";
        return;
    }
    cin.ignore(); // 吸收换行符
    cout << "留言：";
    getline(cin, comment);

    int idx = findDish(dishId);
    if (idx == -1) {
        cout << "菜品不存在\n";
        return;
    }

    // 保存评价记录
    reviewList[reviewCnt].orderId = oid;
    reviewList[reviewCnt].userId = userList[loginIdx].id;
    reviewList[reviewCnt].dishId = dishId;
    reviewList[reviewCnt].star = star;
    reviewList[reviewCnt].comment = comment;
    reviewCnt++;
    // 更新菜品累计评分
    dishList[idx].totalRating += star;
    dishList[idx].ratingCount++;
    saveAll();
    cout << "评价成功\n";
}

// 学生提交投诉建议
void submitComplaint() {
    cout << "\n========== 投诉建议 ==========\n";
    cout << "1. 卫生问题\n2. 服务态度\n3. 菜品质量\n4. 其他\n";
    cout << "选择类型：";
    int t;
    cin >> t;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "输入无效\n";
        return;
    }
    string type;
    switch (t) {
    case 1: type = "卫生问题"; break;
    case 2: type = "服务态度"; break;
    case 3: type = "菜品质量"; break;
    default: type = "其他";
    }

    string content;
    cin.ignore();
    cout << "描述问题：";
    getline(cin, content);

    complaintList[complaintCnt].id = complaintCnt + 1;
    complaintList[complaintCnt].userId = userList[loginIdx].id;
    complaintList[complaintCnt].type = type;
    complaintList[complaintCnt].content = content;
    complaintList[complaintCnt].reply = "待处理";
    complaintList[complaintCnt].status = 0;
    complaintCnt++;
    saveAll();
    cout << "提交成功，编号：" << complaintCnt << endl;
}

// 学生查看自己的投诉记录与回复
void myComplaints() {
    cout << "\n========== 我的投诉 ==========\n";
    bool found = false;
    for (int i = 0; i < complaintCnt; i++) {
        if (complaintList[i].userId == userList[loginIdx].id) {
            found = true;
            cout << "编号：" << complaintList[i].id
                << " 类型：" << complaintList[i].type
                << " 状态：" << (complaintList[i].status ? "已处理" : "待处理") << "\n";
            cout << "问题：" << complaintList[i].content << "\n";
            cout << "回复：" << complaintList[i].reply << "\n";
        }
    }
    if (!found) cout << "暂无投诉\n";
}

// ===================== 管理员端功能函数 =====================
// 取餐核销：输入订单号标记为已取餐
void staffPick() {
    string oid;
    cout << "\n========== 取餐核销 ==========\n";
    cout << "输入订单号：";
    cin >> oid;
    for (int i = 0; i < orderCnt; i++) {
        if (orderList[i].orderId == oid) {
            if (orderList[i].status == 1) {
                orderList[i].status = 2;
                saveAll();
                cout << "核销完成，已取餐\n";
            }
            else if (orderList[i].status == 2) {
                cout << "已取餐\n";
            }
            else {
                cout << "未支付\n";
            }
            return;
        }
    }
    cout << "订单不存在\n";
}

// 新增账号（学生或管理员）
void addUser() {
    cout << "\n========== 新增账号 ==========\n";
    cout << "0.学生 1.员工：";
    int select;
    cin >> select;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "输入无效\n";
        return;
    }

    // 新增学生账号
    if (select == 0) {
        if (userCnt >= MAX_USER) {
            cout << "用户已达上限\n";
            return;
        }
        User u;
        cout << "学号："; cin >> u.id;
        if (findUser(u.id) != -1) {
            cout << "账号已存在\n";
            return;
        }
        cout << "姓名："; cin >> u.name;
        cout << "密码："; cin >> u.pwd;
        cout << "年级："; cin >> u.grade;
        cout << "专业："; cin >> u.major;
        cout << "\n现有班级：\n";
        for (int i = 0; i < classCnt; i++) {
            cout << "  " << classList[i].className << endl;
        }
        cout << "班级："; cin >> u.className;
        cout << "生日："; cin >> u.birth;
        cout << "手机："; cin >> u.number;
        u.money = 0;
        userList[userCnt++] = u;
        updateClassCount();
        saveAll();
        cout << "创建成功\n";
    }
    // 新增员工账号
    else if (select == 1) {
        if (mgrCnt >= MAX_USER) {
            cout << "员工已达上限\n";
            return;
        }
        Manager m;
        cout << "工号："; cin >> m.id;
        for (int i = 0; i < mgrCnt; i++) {
            if (mgrList[i].id == m.id) {
                cout << "账号已存在\n";
                return;
            }
        }
        cout << "姓名："; cin >> m.name;
        cout << "密码："; cin >> m.pwd;
        mgrList[mgrCnt++] = m;
        saveAll();
        cout << "创建成功\n";
    }
}

// 新增菜品
void addDish() {
    if (dishCnt >= MAX_DISH) {
        cout << "菜品已达上限\n";
        return;
    }
    Dish d;
    cout << "\n========== 新增菜品 ==========\n";
    cout << "菜品ID："; cin >> d.id;
    if (findDish(d.id) != -1) {
        cout << "ID已存在\n";
        return;
    }
    cout << "菜名："; cin >> d.name;
    cout << "价格："; cin >> d.price;
    cout << "库存："; cin >> d.stock;
    d.totalRating = 0;
    d.ratingCount = 0;
    dishList[dishCnt++] = d;

    // 新增菜品初始库存记为采购记录
    if (stockCnt < MAX_STOCK) {
        stockList[stockCnt].id = stockCnt + 1;
        stockList[stockCnt].dishId = d.id;
        stockList[stockCnt].quantity = d.stock;
        stockList[stockCnt].opType = "采购";
        stockList[stockCnt].date = currentDate;
        stockList[stockCnt].operatorName = getOperator();
        stockCnt++;
    }
    saveAll();
    cout << "菜品创建成功\n";
}

// 展示班级列表
void showClasses() {
    cout << "\n========== 班级列表 ==========\n";
    cout << left << setw(14) << "班级名"
        << setw(20) << "所属专业"
        << setw(10) << "人数" << endl;
    cout << "----------------------------------------\n";
    for (int i = 0; i < classCnt; i++) {
        cout << left << setw(14) << classList[i].className
            << setw(20) << classList[i].major
            << setw(10) << classList[i].studentCount << endl;
    }
}

// 新增班级
void addClass() {
    if (classCnt >= MAX_CLASS) {
        cout << "班级已达上限\n";
        return;
    }
    ClassInfo c;
    cout << "\n========== 新增班级 ==========\n";
    cout << "班级名："; cin >> c.className;
    if (findClass(c.className) != -1) {
        cout << "班级已存在\n";
        return;
    }
    cout << "所属专业："; cin >> c.major;
    c.studentCount = 0;
    classList[classCnt++] = c;
    saveAll();
    cout << "创建成功\n";
}

// 修改班级信息，修改班级名会同步更新所有学生的班级字段
void modifyClass() {
    cout << "\n========== 修改班级 ==========\n";
    showClasses();
    string oldName;
    cout << "输入要修改的班级名：";
    cin >> oldName;
    int idx = findClass(oldName);
    if (idx == -1) {
        cout << "班级不存在\n";
        return;
    }
    cout << "当前：" << classList[idx].className
        << " / " << classList[idx].major << endl;

    string newName, newMajor;
    cin.ignore();
    cout << "新班级名(直接回车不改)：";
    getline(cin, newName);
    cout << "新专业(直接回车不改)：";
    getline(cin, newMajor);

    // 班级名变更，同步更新所有学生
    if (!newName.empty() && newName != oldName) {
        for (int i = 0; i < userCnt; i++) {
            if (userList[i].className == oldName) {
                userList[i].className = newName;
            }
        }
        classList[idx].className = newName;
    }
    if (!newMajor.empty()) {
        classList[idx].major = newMajor;
    }
    saveAll();
    cout << "修改成功\n";
}

// 统计各班订餐人数与订餐次数
void statClassOrderCount() {
    cout << "\n========== 各班订餐统计 ==========\n";
    cout << left << setw(14) << "班级"
        << setw(14) << "订餐人数"
        << setw(14) << "订餐次数" << endl;
    cout << "----------------------------------------\n";
    for (int c = 0; c < classCnt; c++) {
        int personCnt = 0;
        int orderNum = 0;
        for (int i = 0; i < userCnt; i++) {
            if (userList[i].className == classList[c].className) {
                bool hasOrder = false;
                for (int j = 0; j < orderCnt; j++) {
                    if (orderList[j].userId == userList[i].id && orderList[j].status >= 1) {
                        orderNum++;
                        hasOrder = true;
                    }
                }
                if (hasOrder) personCnt++;
            }
        }
        cout << left << setw(14) << classList[c].className
            << setw(14) << personCnt
            << setw(14) << orderNum << endl;
    }
}

// 按班级查询所有学生的订餐明细
void queryClassOrders() {
    cout << "\n========== 按班级查询 ==========\n";
    showClasses();
    string className;
    cout << "输入班级名：";
    cin >> className;
    if (findClass(className) == -1) {
        cout << "班级不存在\n";
        return;
    }
    cout << "\n【" << className << "】订餐明细：\n";
    bool found = false;
    for (int i = 0; i < userCnt; i++) {
        if (userList[i].className == className) {
            for (int j = 0; j < orderCnt; j++) {
                if (orderList[j].userId == userList[i].id && orderList[j].status >= 1) {
                    found = true;
                    cout << "  学生：" << userList[i].name
                        << " 订单：" << orderList[j].orderId
                        << " 金额：" << orderList[j].total << "元\n";
                    cout << "    菜品：";
                    for (int k = 0; k < orderList[j].itemCnt; k++) {
                        int idx = findDish(orderList[j].list[k].dishId);
                        if (idx != -1) {
                            cout << dishList[idx].name
                                << "x" << orderList[j].list[k].num << " ";
                        }
                    }
                    cout << endl;
                }
            }
        }
    }
    if (!found) cout << "该班级暂无订餐记录\n";
}

// 展示全部学生列表
void showAllUsers() {
    cout << "\n========== 学生列表 ==========\n";
    cout << left << setw(10) << "学号"
        << setw(10) << "姓名"
        << setw(14) << "班级"
        << setw(10) << "余额" << endl;
    cout << "----------------------------------------\n";
    for (int i = 0; i < userCnt; i++) {
        cout << left << setw(10) << userList[i].id
            << setw(10) << userList[i].name
            << setw(14) << userList[i].className
            << setw(10) << userList[i].money << endl;
    }
    cout << "共 " << userCnt << " 名学生\n";
}

// 删除学生账号
void deleteUser() {
    cout << "\n========== 删除学生 ==========\n";
    showAllUsers();
    string id;
    cout << "输入学号：";
    cin >> id;
    int idx = findUser(id);
    if (idx == -1) {
        cout << "学生不存在\n";
        return;
    }
    cout << "确认删除 " << userList[idx].name << "？(1/0)：";
    int confirm;
    cin >> confirm;
    if (confirm != 1) return;

    // 数组元素前移覆盖实现删除
    for (int i = idx; i < userCnt - 1; i++) {
        userList[i] = userList[i + 1];
    }
    userCnt--;
    updateClassCount();
    saveAll();
    cout << "删除成功\n";
}

// 修改学生信息
void modifyUser() {
    cout << "\n========== 修改学生 ==========\n";
    showAllUsers();
    string id;
    cout << "输入学号：";
    cin >> id;
    int idx = findUser(id);
    if (idx == -1) {
        cout << "学生不存在\n";
        return;
    }
    cout << "\n当前信息：\n";
    cout << "姓名：" << userList[idx].name << endl;
    cout << "班级：" << userList[idx].className << endl;
    cout << "余额：" << userList[idx].money << endl;
    cout << "\n1.姓名 2.密码 3.班级 4.余额 0.取消\n";
    int op;
    cin >> op;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "输入无效\n";
        return;
    }
    switch (op) {
    case 1:
        cout << "新姓名："; cin >> userList[idx].name; break;
    case 2:
        cout << "新密码："; cin >> userList[idx].pwd; break;
    case 3:
        showClasses();
        cout << "新班级："; cin >> userList[idx].className;
        updateClassCount(); break;
    case 4:
        cout << "新余额："; cin >> userList[idx].money; break;
    case 0: return;
    default: cout << "无效\n"; return;
    }
    saveAll();
    cout << "修改成功\n";
}

// 按姓名模糊查询学生
void queryUserByName() {
    cout << "\n========== 按姓名查询 ==========\n";
    string name;
    cout << "输入姓名(支持模糊)：";
    cin >> name;
    bool found = false;
    for (int i = 0; i < userCnt; i++) {
        if (userList[i].name.find(name) != string::npos) {
            found = true;
            cout << "学号：" << userList[i].id
                << " 姓名：" << userList[i].name
                << " 班级：" << userList[i].className
                << " 余额：" << userList[i].money << endl;
        }
    }
    if (!found) cout << "未找到\n";
}

// 查询某菜品的所有订购学生与累计销量
void queryDishOrders() {
    cout << "\n========== 菜品订购查询 ==========\n";
    showMenu();
    int dishId;
    cout << "输入菜品ID：";
    cin >> dishId;
    int idx = findDish(dishId);
    if (idx == -1) {
        cout << "菜品不存在\n";
        return;
    }
    cout << "\n订购【" << dishList[idx].name << "】的学生：\n";
    bool found = false;
    int totalQty = 0;
    for (int i = 0; i < orderCnt; i++) {
        if (orderList[i].status < 1) continue; // 只统计已支付订单
        for (int j = 0; j < orderList[i].itemCnt; j++) {
            if (orderList[i].list[j].dishId == dishId) {
                found = true;
                int uidx = findUser(orderList[i].userId);
                string uname = (uidx != -1) ? userList[uidx].name : "未知";
                string uclass = (uidx != -1) ? userList[uidx].className : "未知";
                cout << "  " << uname << "(" << uclass << ") 数量："
                    << orderList[i].list[j].num << " 订单：" << orderList[i].orderId << endl;
                totalQty += orderList[i].list[j].num;
            }
        }
    }
    if (!found) {
        cout << "暂无学生订购\n";
    }
    else {
        cout << "\n累计售出：" << totalQty << " 份\n";
    }
}

// 低库存预警检查
void checkLowStock() {
    cout << "\n========== 低库存预警 ==========\n";
    bool hasWarning = false;
    for (int i = 0; i < dishCnt; i++) {
        if (dishList[i].stock <= LOW_STOCK) {
            hasWarning = true;
            cout << "【预警】" << dishList[i].name
                << " 剩余：" << dishList[i].stock << " 份\n";
        }
    }
    if (!hasWarning) cout << "库存充足\n";
}

// 采购入库，增加菜品库存
void purchaseStock() {
    cout << "\n========== 采购入库 ==========\n";
    showMenu();
    int dishId, qty;
    cout << "输入菜品ID：";
    cin >> dishId;
    int idx = findDish(dishId);
    if (idx == -1) {
        cout << "菜品不存在\n";
        return;
    }
    cout << "当前库存：" << dishList[idx].stock << endl;
    cout << "采购数量：";
    cin >> qty;
    if (cin.fail() || qty <= 0) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "数量必须大于0\n";
        return;
    }
    dishList[idx].stock += qty;
    // 记录采购流水
    if (stockCnt < MAX_STOCK) {
        stockList[stockCnt].id = stockCnt + 1;
        stockList[stockCnt].dishId = dishId;
        stockList[stockCnt].quantity = qty;
        stockList[stockCnt].opType = "采购";
        stockList[stockCnt].date = currentDate;
        stockList[stockCnt].operatorName = getOperator();
        stockCnt++;
    }
    saveAll();
    cout << "采购成功，新库存：" << dishList[idx].stock << endl;
}

// 查看全部库存变动记录
void showStockRecords() {
    cout << "\n========== 库存变动记录 ==========\n";
    cout << left << setw(6) << "ID"
        << setw(14) << "菜品"
        << setw(8) << "类型"
        << setw(8) << "数量"
        << setw(14) << "日期"
        << setw(12) << "操作人" << endl;
    cout << "--------------------------------------------------------\n";
    for (int i = 0; i < stockCnt; i++) {
        int idx = findDish(stockList[i].dishId);
        string dname = (idx != -1) ? dishList[idx].name : "已删除";
        cout << left << setw(6) << stockList[i].id
            << setw(14) << dname
            << setw(8) << stockList[i].opType
            << setw(8) << stockList[i].quantity
            << setw(14) << stockList[i].date
            << setw(12) << stockList[i].operatorName << endl;
    }
    cout << "共 " << stockCnt << " 条记录\n";
}

// 查看菜品评价，可指定菜品或查看全部
void showDishReviews() {
    cout << "\n========== 查看评价 ==========\n";
    showMenu();
    int dishId;
    cout << "输入菜品ID(0查看全部)：";
    cin >> dishId;
    bool found = false;
    for (int i = 0; i < reviewCnt; i++) {
        if (dishId != 0 && reviewList[i].dishId != dishId) continue;
        found = true;
        int idx = findDish(reviewList[i].dishId);
        string dname = (idx != -1) ? dishList[idx].name : "未知";
        int uidx = findUser(reviewList[i].userId);
        string uname = (uidx != -1) ? userList[uidx].name : "未知";
        cout << "  菜品：" << dname
            << " 评分：" << reviewList[i].star << "星"
            << " 学生：" << uname << endl;
        cout << "    留言：" << reviewList[i].comment << endl;
    }
    if (!found) cout << "暂无评价\n";
}

// 查看所有投诉列表
void showComplaints() {
    cout << "\n========== 投诉列表 ==========\n";
    if (complaintCnt == 0) {
        cout << "暂无投诉\n";
        return;
    }
    for (int i = 0; i < complaintCnt; i++) {
        int uidx = findUser(complaintList[i].userId);
        string uname = (uidx != -1) ? userList[uidx].name : "未知";
        cout << "编号：" << complaintList[i].id
            << " 学生：" << uname
            << " 类型：" << complaintList[i].type
            << " 状态：" << (complaintList[i].status ? "已处理" : "待处理") << endl;
        cout << "  问题：" << complaintList[i].content << endl;
        cout << "  回复：" << complaintList[i].reply << endl;
        cout << "----------------------------------------\n";
    }
}

// 处理投诉：填写回复并标记为已处理
void handleComplaint() {
    cout << "\n========== 处理投诉 ==========\n";
    showComplaints();
    int id;
    cout << "输入投诉编号：";
    cin >> id;
    int idx = -1;
    for (int i = 0; i < complaintCnt; i++) {
        if (complaintList[i].id == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        cout << "投诉不存在\n";
        return;
    }
    if (complaintList[idx].status == 1) {
        cout << "已处理\n";
        return;
    }
    string reply;
    cin.ignore();
    cout << "输入回复：";
    getline(cin, reply);
    complaintList[idx].reply = reply;
    complaintList[idx].status = 1;
    saveAll();
    cout << "处理完成\n";
}

// 统计总营收与有效订单数
void showTotal() {
    double sum = 0;
    int cnt = 0;
    for (int i = 0; i < orderCnt; i++) {
        if (orderList[i].status >= 1) {
            sum += orderList[i].total;
            cnt++;
        }
    }
    cout << "\n总营收：" << sum << " 元\n";
    cout << "有效订单：" << cnt << " 单\n";
}

// 菜品销量排行榜（按销量降序）
void showDishSalesRank() {
    int sales[MAX_DISH] = { 0 };
    // 统计每个菜品的总销量
    for (int i = 0; i < orderCnt; i++) {
        if (orderList[i].status < 1) continue;
        for (int j = 0; j < orderList[i].itemCnt; j++) {
            int idx = findDish(orderList[i].list[j].dishId);
            if (idx != -1) sales[idx] += orderList[i].list[j].num;
        }
    }
    // 下标数组，用于冒泡排序
    int rankIdx[MAX_DISH];
    for (int i = 0; i < dishCnt; i++) rankIdx[i] = i;
    // 冒泡排序：按销量从高到低
    for (int i = 0; i < dishCnt - 1; i++) {
        for (int j = 0; j < dishCnt - i - 1; j++) {
            if (sales[rankIdx[j]] < sales[rankIdx[j + 1]]) {
                int t = rankIdx[j];
                rankIdx[j] = rankIdx[j + 1];
                rankIdx[j + 1] = t;
            }
        }
    }

    cout << "\n========== 菜品销量排行 ==========\n";
    cout << left << setw(6) << "排名"
        << setw(14) << "菜名"
        << setw(10) << "销量"
        << setw(12) << "销售额" << endl;
    cout << "----------------------------------------\n";
    for (int i = 0; i < dishCnt; i++) {
        int idx = rankIdx[i];
        double revenue = sales[idx] * dishList[idx].price;
        cout << left << setw(6) << (i + 1)
            << setw(14) << dishList[idx].name
            << setw(10) << sales[idx]
            << setw(12) << revenue << endl;
    }
}

// 综合数据统计概览
void showStatistics() {
    cout << "\n========== 综合统计 ==========\n";
    showTotal();

    // 统计活跃订餐学生数
    int activeStu = 0;
    for (int i = 0; i < userCnt; i++) {
        for (int j = 0; j < orderCnt; j++) {
            if (orderList[j].userId == userList[i].id && orderList[j].status >= 1) {
                activeStu++;
                break;
            }
        }
    }
    cout << "注册学生：" << userCnt << " 人\n";
    cout << "活跃订餐：" << activeStu << " 人\n";
    cout << "班级数量：" << classCnt << " 个\n";
    cout << "菜品数量：" << dishCnt << " 道\n";

    // 统计待处理投诉数
    int pending = 0;
    for (int i = 0; i < complaintCnt; i++) {
        if (complaintList[i].status == 0) pending++;
    }
    cout << "待处理投诉：" << pending << " 条\n";
}

// ===================== 各级菜单函数 =====================
// 班级管理子菜单
void classManageMenu() {
    int op;
    while (true) {
        cout << "\n========== 班级管理 ==========\n";
        cout << "1.查看班级" << endl;
        cout << "2.新增班级" << endl;
        cout << "3.修改班级" << endl;
        cout << "4.订餐统计" << endl;
        cout << "5.班级查询" << endl;
        cout << "0.返回" << endl;
        cout << "选择：";
        cin >> op;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入无效，请输入数字\n";
            continue;
        }
        switch (op) {
        case 1: 
            showClasses(); 
            system("pause");
            system("cls");
            break;
        case 2: 
            addClass();
            system("pause");
            system("cls");
            break;
        case 3: 
            modifyClass();
            system("pause");
            system("cls");
            break;
        case 4:
            statClassOrderCount(); 
            system("pause");
            system("cls");
            break;
        case 5: 
            queryClassOrders();
            system("pause");
            system("cls");
            break;
        case 0: 
            return;
        default: cout << "无效\n";
        }
    }
}

// 学生管理子菜单
void userManageMenu() {
    int op;
    while (true) {
        cout << "\n========== 学生管理 ==========\n";
        cout << "1.查看学生" << endl;
        cout << "2.新增学生" << endl;
        cout << "3.删除学生" << endl;
        cout << "4.修改学生" << endl;
        cout<<"5.按姓名查询"<<endl;
        cout << "6.菜品订购查询" << endl;
        cout << "0.返回\n选择：";
        cin >> op;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入无效，请输入数字\n";
            continue;
        }
        switch (op) {
        case 1: 
            showAllUsers(); 
            system("pause");
            system("cls");
            break;
        case 2: 
            addUser();
            system("pause");
            system("cls");
            break;
        case 3: 
            deleteUser();
            system("pause");
            system("cls");
            break;
        case 4:
            modifyUser();
            system("pause");
            system("cls");
            break;
        case 5:
            queryUserByName(); 
            system("pause");
            system("cls");
            break;
        case 6: 
            queryDishOrders(); 
            system("pause");
            system("cls");
            break;
        case 0: return;
        default: cout << "无效\n";
        }
    }
}

// 库存管理子菜单
void stockManageMenu() {
    int op;
    while (true) {
        cout << "\n========== 库存管理 ==========\n";
        cout << "1.查看菜单" << endl;
        cout << "2.低库存预警" << endl;
        cout << "3.采购入库" << endl;
        cout << "4.库存记录" << endl;
        cout << "5.新增菜品" << endl;
        cout << "0.返回" << endl;
        cout << "选择：";
        cin >> op;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入无效，请输入数字\n";
            continue;
        }
        switch (op) {
        case 1:
            showMenu(); 
            system("pause");
            system("cls");
            break;
        case 2: 
            checkLowStock();
            system("pause");
            system("cls");
            break;
        case 3: 
            purchaseStock();
            system("pause");
            system("cls");
            break;
        case 4: 
            showStockRecords();
            system("pause");
            system("cls");
            break;
        case 5:
            addDish();
            system("pause");
            system("cls");
            break;
        case 0: 
            return;
        default: cout << "无效\n";
        }
    }
}

// 评价与反馈管理子菜单
void feedbackManageMenu() {
    int op;
    while (true) {
        cout << "\n========== 评价与反馈 ==========\n";
        cout << "1.查看评价" << endl;
        cout << "2.查看投诉" << endl;
        cout << "3.处理投诉" << endl;
        cout << "0.返回" << endl;
        cout << "选择：";
        cin >> op;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入无效，请输入数字\n";
            continue;
        }
        switch (op) {
        case 1:
            showDishReviews();
            system("pause");
            system("cls");
            break;
        case 2: 
            showComplaints();
            system("pause");
            system("cls");
            break;
        case 3: 
            handleComplaint();
            system("pause");
            system("cls");
            break;
        case 0: return;
        default: cout << "无效\n";
        }
    }
}

// 数据统计子菜单
void statisticsMenu() {
    int op;
    while (true) {
        cout << "\n========== 数据统计 ==========\n";
        cout << "1.综合概览" << endl;
        cout << "2.总营收" << endl;
        cout << "3.销量排行" << endl;
        cout << "4.订餐统计" << endl;
        cout << "0.返回" << endl;
        cout << "选择：";
        cin >> op;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入无效，请输入数字\n";
            continue;
        }
        switch (op) {
        case 1: 
            showStatistics(); 
            system("pause");
            system("cls");
            break;
        case 2: 
            showTotal(); 
            system("pause");
            system("cls");
            break;
        case 3:
            showDishSalesRank();
            system("pause");
            system("cls");
            break;
        case 4: 
            statClassOrderCount();
            system("pause");
            system("cls");
            break;
        case 0: return;
        default: cout << "无效\n";
        }
    }
}

// 学生端主菜单
void studentMenu() {
    system("pause");
    system("cls");
    int op;
    while (true) {
        cout << "\n========== 学生菜单 ==========\n";
        cout << "1.查看菜单" << endl;
        cout << "2.点餐" << endl;
        cout << "3.充值" << endl;
        cout << "4.我的订单" << endl;
        cout << "5.评价" << endl;
        cout << "6.投诉" << endl;
        cout << "7.我的投诉" << endl;
        cout << "8.切换账号" << endl;
        cout << "0.退出\n" << endl;
        cout << "选择：";
        cin >> op;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入无效，请输入数字\n";
            continue;
        }
        switch (op) {
        case 1: 
            showMenu();
            system("pause");
            system("cls");
            break;
        case 2: 
            studentOrder();
            system("pause");
            system("cls");
            break;
        case 3:
            studentRecharge(); 
            system("pause");
            system("cls");
            break;
        case 4: 
            myOrders();
            system("pause");
            system("cls");
            break;
        case 5: 
            addReview();
            system("pause");
            system("cls");
            break;
        case 6: 
            submitComplaint();
            system("pause");
            system("cls");
            break;
        case 7:
            myComplaints(); 
            system("pause");
            system("cls");
            break;
        case 8:
            system("cls");
            saveAll();
            loginIdx = login();
            if (loginIdx == -1) exit(0);
            return;
        case 0:
            saveAll();
            cout << "再见\n";
            exit(0);
        default: cout << "无效\n";
        }
    }
}

// 管理员端主菜单
void managerMenu() {
    system("pause");
    system("cls");
    int op;
    while (true) {
        cout << "\n========== 管理员菜单 ==========\n";
        cout << "1.查看菜单" << endl;
        cout << "2.取餐核销" << endl;
        cout << "3.班级管理" << endl;
        cout << "4.学生管理" << endl;
        cout << "5.库存管理" << endl;
        cout << "6.评价反馈" << endl;
        cout << "7.数据统计" << endl;
        cout << "8.切换账号" << endl;
        cout << "0.退出" << endl;
        cout << "选择：";
        cin >> op;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入无效，请输入数字\n";
            continue;
        }
        switch (op) {
        case 1: 
            showMenu();
            system("pause");
            system("cls");
            break;
        case 2: 
            staffPick(); 
            system("pause");
            system("cls");
            break;
        case 3:
            classManageMenu();
            system("pause");
            system("cls");
            break;
        case 4: 
            userManageMenu();
            system("pause");
            system("cls");
            break;
        case 5: 
            stockManageMenu(); 
            system("pause");
            system("cls");
            break;
        case 6: 
            feedbackManageMenu();
            system("pause");
            system("cls");
            break;
        case 7: 
            statisticsMenu();
            system("pause");
            system("cls");
            break;
        case 8:
            system("cls");
            saveAll();
            loginIdx = login();
            if (loginIdx == -1) exit(0);
            return;
        case 0:
            saveAll();
            cout << "再见\n";
            exit(0);
        default: cout << "无效\n";
        }
    }
}

// 总菜单分发：根据角色进入对应菜单
void mainMenu() {
    while (true) {
        if (isManager) {
            managerMenu();
        }
        else {
            studentMenu();
        }
    }
}

// ===================== 程序主入口 =====================
int main() {
    init(); // 系统初始化
    cout << "========================================" << endl;
    cout << "           校园食堂订餐管理系统         " << endl;
    cout << "========================================" << endl;

    // 登录入口，支持重试
jixv:loginIdx = login();
    while(loginIdx == -1) {
        cout << "登录失败\n";
        cout << "是否重新登录？(1继续,0退出)" << endl;
        int con;
        cin >> con;
        system("cls");
        if (con == 1) {
            goto jixv; // 跳回登录
        }
        else {
            goto wanbi; // 跳转到退出
        }
        return 0;
    }
    mainMenu(); // 进入主菜单循环

wanbi:cout << "再见！" << endl;
    return 0;
}
