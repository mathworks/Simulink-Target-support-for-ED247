updateGitlabCommitStatus state: 'pending'

releases = [
    'r2016b',
    'r2017b',
    'r2018b',
    'r2019b',
	'r2020b'
]

pipeline {

	agent { label 'WIN' }

	options {
		gitLabConnection('Consulting FR')
	}

    environment {

        USERPROFILE = 'C:\\Temp'
        MW_MINGW64_LOC = '\\\\gnb-csg-win01\\c$\\MinGW\\4.9.2-posix'
        ED247_LOC = 'C:\\Temp\\ed247'
        LIBXML_LOC = 'C:\\Temp\\libxml2'

		// cf http://inside.mathworks.com/wiki/Using_MW_CRASH_MODE
		MW_CRASH_MODE = 'none'

	}

	stages {
        
		stage ('Tests and packages'){

			steps {
				script {
					def jobPerRelease = [:]
					releases.each { release ->
						jobPerRelease[release] = pipelineByRelease(release)
					}
					parallel jobPerRelease
				}
			}

		}
		
		stage('Publish') {

			agent {
				label 'LINUX'
			}
			steps {

				script {

					catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS') {
						sh """
						rm *.tap
						"""
					}

					releases.each { release ->
						publishResults(release)
					}
					step([$class: "TapPublisher", testResults: "*.tap"])
					
                    catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS') { 
                        cobertura coberturaReportFile: '*.xml'			
                    }

				}
			}			
		}
		
	}

	post {
		always {
			archiveArtifacts allowEmptyArchive: true, artifacts: '*.mltbx'
			emailext attachLog: true, attachmentsPattern: '*.pdf',
				body: "${currentBuild.currentResult}: Job ${env.JOB_NAME} build ${env.BUILD_NUMBER}\n More info at: ${env.BUILD_URL}",
				recipientProviders: [developers(), requestor()],
				subject: "Jenkins Build ${currentBuild.currentResult}: Job ${env.JOB_NAME}"			
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

def pipelineByRelease(release){

	return { 

		node("WIN"){

            stage("$release") {

                checkout scm

				catchError(buildResult: 'FAILURE', stageResult: 'FAILURE') {
					bat """
					mw -using $release:perfect matlab -wait -logfile "C:%WORKSPACE%/output.log" -r "exit(ci.Pipeline.forFolder())"
					"""
				}
				catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS') { 
					bat """
					type "C:%WORKSPACE:/=\\%\\output.log"
					"""
				}
				catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS') {
					stash includes: "*.mltbx,*.tap,*.pdf,*.xml", name: "$release", allowEmpty: true
				}

			}

		}

	}

}

def publishResults(release) {

	catchError(buildResult: 'SUCCESS', message: 'unstash') {
		unstash name: "$release"
	}

}