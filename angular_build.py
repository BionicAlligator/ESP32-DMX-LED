import os
Import("env")

def pre_program_action(source, target, env):
  print("HERE!")
  os.chdir("dmx-web-app")
  env.Execute("ng build")
  env.Execute(Delete('../data/www/'))
  env.Execute(Copy("../data/www/","dist/dmx-web-app/browser/"))
  os.chdir("..")

print("Registering custom Angular build step to SPIFFS build")
#env.AddPreAction(["mkspiffs_espressif32_arduino", "buildfs", "uploadfs"], pre_program_action)