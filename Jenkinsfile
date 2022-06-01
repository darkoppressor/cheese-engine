pipeline {
    agent any

    stages {
        stage('build') {
            steps {
                sh "python3 -m venv . ; . bin/activate ; pip3 install --upgrade pip feedgen ; python3 ci-cd.py ${env.JOB_NAME} ${env.BUILD_NUMBER} ${env.BUILD_URL}"
            }
        }
    }
}
