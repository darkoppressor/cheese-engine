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
                slackSend message: "Build started - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"

                sh 'chmod +x tools/*'
                sh 'chmod +x tools/build-system/*'
                sh 'chmod +x tools/build-system/scripts/*'
                sh 'chmod -x tools/build-system/scripts/*-x86_64'
                sh 'chmod -x tools/build-system/scripts/*.cmake'
                sh 'chmod +x tools/build-system/scripts/android/*'
                sh 'chmod +x development/android/android-prep'
                sh 'chmod +x development/android/clean-android'

                sh 'tools/build-system/scripts/build-engine $(pwd)'
            }
        }
    }
}
