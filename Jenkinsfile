pipeline {
    agent any

    stages {
        stage('build') {
            steps {
                sh '../make_executable'
                sh 'tools/build-system/scripts/build-engine $(pwd)'
            }
        }
    }
}
