pipeline {
    agent any

    stages {
        stage('build') {
            steps {
                sh "tools/engineCiCd ${env.JOB_NAME} ${env.BUILD_NUMBER} ${env.BUILD_URL}"
            }
        }
    }
}
