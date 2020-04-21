# Import statements
import os
from dotenv import load_dotenv
from api import app

# Load server virtual environment into Python path
proj_folder = os.path.expanduser('~/futurewallet')
load_dotenv(os.path.join(proj_folder, '.env'))

# Main WSGI driver block
if __name__ == '__main__':
    app.run()
