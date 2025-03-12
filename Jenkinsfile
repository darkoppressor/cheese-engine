pipeline {
    agent any

    stages {
        stage('build') {
            steps {
                sh "/home/tails/build-server/cheese-engine/tools/engineCiCd ${env.JOB_NAME} ${env.BUILD_NUMBER} ${env.BUILD_URL}"
            }
        }
    }
}
