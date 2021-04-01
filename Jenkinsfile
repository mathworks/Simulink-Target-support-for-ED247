updateGitlabCommitStatus state: 'pending'

releases = [
	'r2019b'
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
					def tests = [:]
					releases.each { release ->
						pip[release] = pipelineByRelease(release)
					}
					parallel pip
				}
			}

		}
		
		stage('Publish TAP results') {

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
					
				}
			}			
		}
		
	}

	post {
		always {
			archiveArtifacts allowEmptyArchive: true, artifacts: '*.mltlbx'
			emailext attachLog: true, attachmentsPattern: 'TestReport.pdf,*.mltlbx',
				body: "${currentBuild.currentResult}: Job ${env.JOB_NAME} build ${env.BUILD_NUMBER}\n More info at: ${env.BUILD_URL}",
				recipientProviders: [developers(), requestor()],
				subject: "Jenkins Build ${currentBuild.currentResult}: Job ${env.JOB_NAME}"
			cobertura coberturaReportFile: 'CoverageResults.xml'			
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
		
			stage("Prepare $release") {

				catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE') {
					bat """
					mw -using $release:perfect matlab -wait -logfile C:%WORKSPACE%/prepare.$release.log -r "exit(ci.prepare())"
					"""
				}
				catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS') { 
					bat """
					type "C:%WORKSPACE:/=\\%\\prepare.$release.log"
					"""
				}

			}

			stage("Test $release") {

				catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE') {
					bat """
					mw -using $release:perfect matlab -wait -logfile C:%WORKSPACE%/test.$release.log -r "exit(ci.test('TAPFile','$release.tap'))"
					"""
				}
				catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS') { 
					bat """
					type "C:%WORKSPACE:/=\\%\\test.$release.log"
					"""
				}
				catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS') {
					stash includes: "$release.tap", name: "$release"
				}

			}
			
			stage("Package $release") {

				catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE') {
					bat """
					mw -using $release:perfect matlab -wait -logfile C:%WORKSPACE%/package.$release.log -r "exit(ci.package())"
					"""
				}
				catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS') { 
					bat """
					type "C:%WORKSPACE:/=\\%\\package.$release.log"
					"""
				}

			}
			
			stage("Cleanup $release") {

				catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE') {
					bat """
					mw -using $release:perfect matlab -wait -logfile C:%WORKSPACE%/cleanup.$release.log -r "exit(ci.cleanup())"
					"""
				}
				catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS') { 
					bat """
					type "C:%WORKSPACE:/=\\%\\cleanup.$release.log"
					"""
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