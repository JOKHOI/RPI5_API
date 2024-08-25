import subprocess
import sys
def install_package_with_pip(package_name):
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", package_name])
        print(f"{package_name} installed successfully with pip.")
    except subprocess.CalledProcessError:
        return False  # Indicate failure
    return True  # Indicate success

def install_package_with_apt(package_name):
    try:
        # The command might require the user to enter their password for sudo
        subprocess.check_call(["sudo", "apt", "install", package_name, "-y"])
        print(f"{package_name} installed successfully with apt.")
    except subprocess.CalledProcessError as e:
        print(f"Failed to install {package_name} with apt. Error: {e}")

    packages = {
        "matplotlib": "python3-matplotlib",
        "pynput": "python3-pynput"
    }

    for pip_name, apt_name in packages.items():
        success = install_package_with_pip(pip_name)
        if not success:
            print(f"Attempting to install {pip_name} with apt...")
            install_package_with_apt(apt_name)