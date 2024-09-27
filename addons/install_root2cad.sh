# NOTE: If the script execution fails, try executing one by one the steps
# 1. Execute -> curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.1/install.sh | bash
# 2. CLose and reopen a terminal, since the above modifies the bash.rc and is renew for user session
# 3. source nvm install node -> it takes a while ~2h
# 4. nvm install --lts
# 5. sudo apt-get install xvfb
# 6. npm install -g root2cad
#Install nvm to get npm and node -> prerequisite
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.1/install.sh | bash
nvm install node
nvm install --lts
sudo apt-get install xvfb
npm install -g root2cad
