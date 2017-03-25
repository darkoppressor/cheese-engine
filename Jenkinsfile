pipeline {
    agent any

    stages {
        stage('build') {
            post {
                success {
                    slackSend color: 'good', message: "Build SUCCEEDED - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                }
                failure {
                    slackSend color: 'danger', message: "Build FAILED - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                }
                unstable {
                    slackSend color: 'warning', message: "Build was UNSTABLE - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"
                }
            }

            steps {
                slackSend message: "Build started - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)"

                sh 'chmod +x tools/*'
                sh 'chmod +x tools/build-system/*'
                sh 'chmod +x tools/build-system/scripts/*'
                sh 'chmod -x tools/build-system/scripts/*-x86_64'
                sh 'chmod -x tools/build-system/scripts/*-x86'
                sh 'chmod -x tools/build-system/scripts/*.cmake'
                sh 'chmod +x tools/build-system/scripts/android/*'
                sh 'chmod +x development/android/android-prep'
                sh 'chmod +x development/android/clean-android'

                sh 'tools/build-system/scripts/build-engine $(pwd)'
            }
        }
    }
}
