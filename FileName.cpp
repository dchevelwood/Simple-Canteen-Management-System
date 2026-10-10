#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
using namespace std;

const int MAX_DISH = 50;
const int MAX_USER = 500;
const int MAX_ORDER = 1000;
const int MAX_CLASS = 50;
const int MAX_REVIEW = 1000;
const int MAX_COMPLAINT = 500;
const int MAX_STOCK = 2000;
const int LOW_STOCK = 10;
const int MAX_ORDER_ITEM = 10;

struct Dish {
    int id;
    string name;
    double price;
    int stock;
    double totalRating;
    int ratingCount;
};

struct ClassInfo {
    string className;
    string major;
    int studentCount;
};

struct User {
    string id;
    string name;
    string pwd;
    int grade;
    string major;
    string className;
    string birth;
    string number;
    double money;
};

struct Manager {
    string id;
    string name;
    string pwd;
};

struct BuyItem {
    int dishId;
    int num;
};

struct Order {
    string orderId;
    string userId;
    BuyItem list[MAX_ORDER_ITEM];
    int itemCnt;  
    double total;
    int status;
};

struct Review {
    string orderId;
    string userId;
    int dishId;
    int star;
    string comment;
};

struct Complaint {
    int id;
    string userId;
    string type;
    string content;
    string reply;
    int status;
};

struct StockRecord {
    int id;
    int dishId;
    int quantity;
    string opType;
    string date;
    string operatorName;
};


Manager mgrList[MAX_USER];
int mgrCnt = 0;

Dish dishList[MAX_DISH];
int dishCnt = 0;

User userList[MAX_USER];
int userCnt = 0;

Order orderList[MAX_ORDER];
int orderCnt = 0;

ClassInfo classList[MAX_CLASS];
int classCnt = 0;

Review reviewList[MAX_REVIEW];
int reviewCnt = 0;

Complaint complaintList[MAX_COMPLAINT];
int complaintCnt = 0;

StockRecord stockList[MAX_STOCK];
int stockCnt = 0;

int loginIdx = -1;
bool isManager = false;
string currentDate = "2026-07-21";

string intToStr(int num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

int findDish(int id) {
    for (int i = 0; i < dishCnt; i++) {
        if (dishList[i].id == id) return i;
    }
    return -1;
}

int findUser(string id) {
    for (int i = 0; i < userCnt; i++) {
        if (userList[i].id == id) return i;
    }
    return -1;
}

int findClass(string name) {
    for (int i = 0; i < classCnt; i++) {
        if (classList[i].className == name) return i;
    }
    return -1;
}

string getOperator() {
    if (isManager) {
        return mgrList[loginIdx].name;
    }
    else {
        return userList[loginIdx].name;
    }
}

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
 

void saveAll() {
    ofstream f;

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

    f.open("data_manager.txt");
    f << mgrCnt << endl;
    for (int i = 0; i < mgrCnt; i++) {
        f << mgrList[i].id << " "
            << mgrList[i].name << " "
            << mgrList[i].pwd << endl;
    }
    f.close();

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

    f.open("data_class.txt");
    f << classCnt << endl;
    for (int i = 0; i < classCnt; i++) {
        f << classList[i].className << " "
            << classList[i].major << " "
            << classList[i].studentCount << endl;
    }
    f.close();

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

void loadAll() {
    ifstream f;

    f.open("data_dish.txt");
    if (f.is_open()) {
        f >> dishCnt;
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
            for (int j = 0; j < orderList[i].itemCnt; j++) {
                f >> orderList[i].list[j].dishId
                    >> orderList[i].list[j].num;
                if (f.fail()) break;
            }
        }
    }
    f.close();

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


void init() {
    loadAll();

    if (dishCnt == 0) {
        dishList[dishCnt++] = { 1, "红烧肉饭", 15, 50, 0, 0 };
        dishList[dishCnt++] = { 2, "黄焖鸡", 16, 30, 0, 0 };
        dishList[dishCnt++] = { 3, "麻辣香锅", 25, 20, 0, 0 };
        dishList[dishCnt++] = { 4, "番茄炒蛋", 10, 40, 0, 0 };
        dishList[dishCnt++] = { 5, "宫保鸡丁", 14, 35, 0, 0 };
    }

    if (classCnt == 0) {
        classList[classCnt++] = { "计科2301", "计算机科学与技术", 0 };
        classList[classCnt++] = { "软工2302", "软件工程", 0 };
        classList[classCnt++] = { "智科2301", "智能科学与技术", 0 };
    }

    if (userCnt == 0) {
        userList[userCnt++] = { "stu001", "小明", "123", 7, "计算机科学与技术", "计科2301", "2000-01-01", "12345678901", 100.0 };
        userList[userCnt++] = { "stu002", "小红", "123", 7, "软件工程", "软工2302", "2001-03-15", "13900001111", 80.0 };
    }

    if (mgrCnt == 0) {
        mgrList[mgrCnt++] = { "sta001", "打饭阿姨", "123" };
        mgrList[mgrCnt++] = { "man001", "管理员", "123" };
    }

    updateClassCount();
}


int login() {
    string id, pwd;
    cout << "\n========== 登录 ==========\n";
    cout << "账号：";
    cin >> id;
    cout << "密码：";
    cin >> pwd;

    for (int i = 0; i < userCnt; i++) {
        if (userList[i].id == id && userList[i].pwd == pwd) {
            cout << "欢迎学生 " << userList[i].name << endl;
            isManager = false;
            return i;
        }
    }

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

        if (dishList[i].ratingCount > 0) {
            cout << setw(10) << avg;
        }
        else {
            cout << setw(10) << "暂无";
        }

        if (dishList[i].stock <= LOW_STOCK) {
            cout << " [库存不足!]";
        }
        cout << endl;
    }
}


void studentOrder() {
    showMenu();

    Order ord;
    ord.orderId = "ORD" + intToStr(1000 + orderCnt);
    ord.userId = userList[loginIdx].id;
    ord.itemCnt = 0;
    ord.total = 0;
    ord.status = 0;

    int dishId, num;
    while (true) {
        if (ord.itemCnt >= MAX_ORDER_ITEM) {
            cout << "\n订单菜品已达上限(" << MAX_ORDER_ITEM << "种)\n";
            break;
        }
        cout << "\n输入菜品ID(0结束)：";
        cin >> dishId;
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
        if (cin.fail() || num <= 0 || num > dishList[idx].stock) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "数量无效或库存不足\n";
            continue;
        }

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

    if (userList[loginIdx].money < ord.total) {
        cout << "余额不足\n";
        return;
    }

    userList[loginIdx].money -= ord.total;

    for (int i = 0; i < ord.itemCnt; i++) {
        int idx = findDish(ord.list[i].dishId);
        if (idx != -1) {
            dishList[idx].stock -= ord.list[i].num;

            if (stockCnt < MAX_STOCK) {
                stockList[stockCnt].id = stockCnt + 1;
                stockList[stockCnt].dishId = ord.list[i].dishId;
                stockList[stockCnt].quantity = ord.list[i].num;
                stockList[stockCnt].opType = "消耗";
                stockList[stockCnt].date = currentDate;
                stockList[stockCnt].operatorName = userList[loginIdx].name;
                stockCnt++;
            }

            if (dishList[idx].stock <= LOW_STOCK) {
                cout << "【预警】" << dishList[idx].name
                    << " 库存仅剩 " << dishList[idx].stock << " 份\n";
            }
        }
    }

    ord.status = 1;
    orderList[orderCnt++] = ord;
    saveAll();
    cout << "支付成功！订单号：" << ord.orderId << endl;
}

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

void addReview() {
    cout << "\n========== 评价 ==========\n";
    myOrders();

    string oid;
    cout << "输入订单号：";
    cin >> oid;

    int oi = -1;
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

    cin.ignore();
    cout << "留言：";
    getline(cin, comment);

    int idx = findDish(dishId);
    if (idx == -1) {
        cout << "菜品不存在\n";
        return;
    }

    reviewList[reviewCnt].orderId = oid;
    reviewList[reviewCnt].userId = userList[loginIdx].id;
    reviewList[reviewCnt].dishId = dishId;
    reviewList[reviewCnt].star = star;
    reviewList[reviewCnt].comment = comment;
    reviewCnt++;

    dishList[idx].totalRating += star;
    dishList[idx].ratingCount++;

    saveAll();
    cout << "评价成功\n";
}

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

    for (int i = idx; i < userCnt - 1; i++) {
        userList[i] = userList[i + 1];
    }
    userCnt--;
    updateClassCount();
    saveAll();
    cout << "删除成功\n";
}

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
        if (orderList[i].status < 1) continue;
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

void showDishSalesRank() {
    int sales[MAX_DISH] = { 0 };

    for (int i = 0; i < orderCnt; i++) {
        if (orderList[i].status < 1) continue;
        for (int j = 0; j < orderList[i].itemCnt; j++) {
            int idx = findDish(orderList[i].list[j].dishId);
            if (idx != -1) sales[idx] += orderList[i].list[j].num;
        }
    }

    int rankIdx[MAX_DISH];
    for (int i = 0; i < dishCnt; i++) rankIdx[i] = i;

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

void showStatistics() {
    cout << "\n========== 综合统计 ==========\n";
    showTotal();

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

    int pending = 0;
    for (int i = 0; i < complaintCnt; i++) {
        if (complaintList[i].status == 0) pending++;
    }
    cout << "待处理投诉：" << pending << " 条\n";
}

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

int main() {
    init();

    cout << "========================================" << endl;
    cout << "           校园食堂订餐管理系统         " << endl;
    cout << "========================================" << endl;

    jixv:loginIdx = login();
    while(loginIdx == -1) {
        cout << "登录失败\n";
        cout << "是否重新登录？(1继续,0退出)" << endl;
        int con;
        cin >> con;
        system("cls");
        if (con == 1) {
            goto jixv;
        }
        else {
            goto wanbi;     
        }
        return 0;
    }
    mainMenu();
    wanbi:cout << "再见！" << endl;
    return 0;
}
