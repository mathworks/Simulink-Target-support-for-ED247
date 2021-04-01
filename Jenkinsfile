updateGitlabCommitStatus state: 'pending'
pipeline {

	agent { label 'WIN' }

	options {
		gitLabConnection('Consulting FR')
	}

    environment {

		MATLAB_RELEASE = 'r2019b'

        USERPROFILE = 'C:\\Temp'
        MW_MINGW64_LOC = '\\\\gnb-csg-win01\\c$\\MinGW\\4.9.2-posix'
        ED247_LOC = 'C:\\Temp\\ed247'
        LIBXML_LOC = 'C:\\Temp\\libxml2'

		// cf http://inside.mathworks.com/wiki/Using_MW_CRASH_MODE
		MW_CRASH_MODE = 'none'

	}

	stages {

        stage('Prepare'){
            steps {
                bat """
                mw -using %MATLAB_RELEASE%:perfect matlab -wait -logfile C:%WORKSPACE%/prepare.log -r "exit(ci.prepare())"
				type "C:%WORKSPACE:/=\\%\\prepare.log"
                """
            }
        }
        
        stage('Test') {
            steps {
                bat """
                mw -using %MATLAB_RELEASE%:perfect matlab -wait -logfile C:%WORKSPACE%/test.log -r "exit(ci.test())"
				type "C:%WORKSPACE:/=\\%\\test.log"
                """
            }
        }

        stage('Cleanup'){
            steps {
                bat """
                mw -using %MATLAB_RELEASE%:perfect matlab -wait -logfile C:%WORKSPACE%/cleanup.log -r "exit(ci.cleanup())"
				type "C:%WORKSPACE:/=\\%\\cleanup.log"
                """
            }
        }

/*
        stage('Package'){
            when {
                branch "master"
            }
            steps {
                bat """
                mw -using %MATLAB_RELEASE%:perfect matlab -r "exit(ci.package())"
                """
            }
        }
*/
	}

	post {
		always {
			archiveArtifacts allowEmptyArchive: true, artifacts: '*.mltlbx'
			emailext attachLog: true, attachmentsPattern: 'TestReport.pdf,*.mltlbx',
				body: "${currentBuild.currentResult}: Job ${env.JOB_NAME} build ${env.BUILD_NUMBER}\n More info at: ${env.BUILD_URL}",
				recipientProviders: [developers(), requestor()],
				subject: "Jenkins Build ${currentBuild.currentResult}: Job ${env.JOB_NAME}"
			cobertura coberturaReportFile: 'CoverageResults.xml'
			step([$class: "TapPublisher", testResults: "TAPResults.tap"])
			cleanWs cleanWhenAborted: false, cleanWhenFailure: false, cleanWhenNotBuilt: false, cleanWhenUnstable: false, notFailBuild: true, deleteDirs:true
		}
		success {
			updateGitlabCommitStatus name: 'build', state: 'success'
		}
		failure {
			updateGitlabCommitStatus name: 'build', state: 'failed'
		}
		aborted  {
			updateGitlabCommitStatus name: 'build', state: 'canceled'
		}
	}

}