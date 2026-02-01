// 不用继承接口 防止多余开销
class Danmaku{
public:
    //Danmaku b = new Danmaku(a)
    //b = a
    Danmaku() = default;

    Danmaku(const Danmaku& other){

    }

    Danmaku& operator=(const Danmaku& other){

    }

    // 实现update方法

    // 实现renderable方法

    // 实现碰撞体的方法

    //各种get set 方法

    ~Danmaku() = default; 

private:

// 弹幕的坐标 (vector<glm::vec2>) 为了让有些多坐标弹幕能够确定位置(比如激光需要起始坐标与终止坐标)
    
// image 负责弹幕的贴图

// 碰撞体接口 负责弹幕的碰撞检测 

// 弹幕的类型

// 运动函数接口

// 附加数据 使用Vector<float> 

// 是否active(比如是否已经碰撞 不active的弹幕不会更新与进行碰撞检测)

// 


};