//编译命令: 
// g++ test.cpp -o test -lglut -lGL 
// 如果遇到了创建上下文错误的问题, 运行如下代码
// export DISPLAY=:0
// export LIBGL_ALWAYS_INDIRECT=0
#include <GL/glut.h>

// 添加旋转角度变量
float rotationAngle = 0.0f;

void render() {
    // 设置背景色：矢车菊蓝
    glClearColor(0.392, 0.584, 0.930, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 保存当前矩阵状态
    glPushMatrix();
    
    // 应用旋转变换
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    
    // 绘制三角形（几何形状）
    glBegin(GL_TRIANGLES);
    // 使用固定位置的颜色，不随旋转变化
    glColor3f(1.0, 0.0, 0.0); // 右下角始终为红色
    glVertex2f(0.5, -0.5);   
    
    glColor3f(0.0, 1.0, 0.0); // 左下角始终为绿色
    glVertex2f(-0.5, -0.5);  
    
    glColor3f(0.0, 0.0, 1.0); // 顶部始终为蓝色
    glVertex2f(0.0, 0.5);    
    glEnd();
    
    // 恢复矩阵状态
    glPopMatrix();

    // 渲染图片
    glutSwapBuffers();
}

// 添加计时器回调函数
void update(int value) {
    // 更新旋转角度
    rotationAngle += 1.0f;
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }
    
    // 触发重绘
    glutPostRedisplay();
    
    // 重新设置计时器
    glutTimerFunc(16, update, 0); // 约60帧每秒
}

int main(int argc, char** argv) {
    // 初始化GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 修改为双缓冲
    glutInitWindowPosition(60, 60);
    glutInitWindowSize(640, 480);
    glutCreateWindow("PA2 Immediate Mode");
    
    // 设置渲染回调函数
    glutDisplayFunc(render);
    
    // 设置计时器回调函数
    glutTimerFunc(16, update, 0);
    
    // 启动主循环
    glutMainLoop();
    return 0;
}