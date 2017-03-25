pipeline {
    agent any

    stages {
        stage('build') {
            steps {
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
