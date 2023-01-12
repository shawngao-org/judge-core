pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
                sh 'bash build.sh'
            }
        }
        stage('Test'){
            steps {
                sh 'echo test'
            }
        }
        stage('Deploy') {
            steps {
                sh 'echo publish'
            }
        }
    }
}