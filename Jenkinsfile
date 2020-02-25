pipeline {
    agent any

    stages {
        stage('build') {
            post {
                success {
                    slackSend color: 'good', message: "Build SUCCEEDED - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                    sh "/home/tails/server/sendmail '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - SUCCEEDED!' '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - SUCCEEDED:\n\nCheck console output at ${env.BUILD_URL} to view the results.' 'darkoppressor@gmail.com'"
                }
                failure {
                    slackSend color: 'danger', message: "Build FAILED - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                    sh "/home/tails/server/sendmail '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - FAILED!' '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - FAILED:\n\nCheck console output at ${env.BUILD_URL} to view the results.' 'darkoppressor@gmail.com'"
                }
                unstable {
                    slackSend color: 'warning', message: "Build UNSTABLE - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                    sh "/home/tails/server/sendmail '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - UNSTABLE!' '${env.JOB_NAME} - Build # ${env.BUILD_NUMBER} - UNSTABLE:\n\nCheck console output at ${env.BUILD_URL} to view the results.' 'darkoppressor@gmail.com'"
                }
            }

            steps {
                sh "python3 -m venv . ; . bin/activate ; pip3 install --upgrade pip feedgen ; python3 ci-cd.py ${env.JOB_NAME} ${env.BUILD_NUMBER} ${env.BUILD_URL}"
            }
        }
    }
}
