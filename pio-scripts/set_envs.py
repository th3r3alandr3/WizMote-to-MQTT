from os.path import isfile
Import("env")

assert isfile(".env")

try:
    with open(".env", "r") as f:
        lines = f.readlines()
        envs = []
        for line in lines:
            env_line = line.strip()
            if env_line.startswith("#") or not env_line:
                continue
            key, value = env_line.split("=", 1)
            escaped_value = value.strip().replace('"', '\\"')
            quoted_value = f'"{escaped_value}"'
            print(f"Setting {key} to {quoted_value}")  # Debug print
            envs.append(f"-D{key}={quoted_value}")
        env.Append(BUILD_FLAGS=envs)
except IOError:
    print("File .env not accessible")