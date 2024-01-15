import subprocess
import time

def run_another_script():
    try:
        # 在这里替换成你想要运行的脚本的路径
        script_path = 'C:/python programs/closed-loop control.py'
        
        # 使用subprocess运行脚本
        subprocess.run(['python', script_path])
    except Exception as e:
        print(f"Error running script: {e}")

# 循环运行的次数，可以根据需要更改
num_runs = 10

# 循环
for i in range(num_runs):
    print(f"Running script iteration {i + 1}")
    
    # 调用运行另一个脚本的函数
    run_another_script()
    
    # 等待一段时间，可以根据需要更改
    time.sleep(1)  # 等待1秒钟