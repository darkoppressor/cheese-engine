import sys
import argparse
import logging
import os
import subprocess

sys.path.append('/home/tails/server/feeds/')
import feeds

logging.basicConfig(level = logging.WARNING)

argparser = argparse.ArgumentParser(description = 'Execute CI/CD steps.')
argparser.add_argument('ciJobName',
                        help = 'the name of the CI job')
argparser.add_argument('ciBuildNumber',
                        help = 'the build number of the CI job')
argparser.add_argument('ciBuildUrl',
                        help = 'the build URL of the CI job')

def updateFeed (prefix, ciJobName, ciBuildNumber, ciBuildUrl):
    title = prefix + ciJobName + ' ' + ciBuildNumber
    content = title + ' (<a href="' + ciBuildUrl + '">Open</a>)'
    feeds.post('jenkins', 'Wells Family Jenkins', 'Tails', 'tails.at.mobius@gmail.com', 'https://wells-family.xyz/', 'https://wells-family.xyz/favicon.png', title, content)

def setFilesExecutable (directory, excludedSuffixes = None):
    for fileName in os.listdir(directory):
        shouldExclude = False
        if excludedSuffixes is not None:
            for excludedSuffix in excludedSuffixes:
                if fileName.endswith(excludedSuffix):
                    shouldExclude = True
                    break

        if not shouldExclude:
            os.chmod(directory + '/' + fileName, 0o755)

def main (argv):
    args = argparser.parse_args()

    ciJobName = args.ciJobName
    ciBuildNumber = args.ciBuildNumber
    ciBuildUrl = args.ciBuildUrl

    try:
        updateFeed('Build started - ', ciJobName, ciBuildNumber, ciBuildUrl)

        setFilesExecutable('tools')
        setFilesExecutable('tools/build-system')
        setFilesExecutable('tools/build-system/scripts', ['-x86_64', '.cmake'])
        setFilesExecutable('tools/build-system/scripts/android')
        os.chmod('development/android/android-prep', 0o755)
        os.chmod('development/android/clean-android', 0o755)

        if subprocess.run(['tools/build-system/scripts/build-engine', os.getcwd()]):
            updateFeed('Build completed successfully - ', ciJobName, ciBuildNumber, ciBuildUrl)

            if subprocess.run(['tools/build-system/scripts/deploy-engine', os.getcwd()]):
                updateFeed('Deployment completed successfully - ', ciJobName, ciBuildNumber, ciBuildUrl)
            else:
                updateFeed('Deployment failed - ', ciJobName, ciBuildNumber, ciBuildUrl)
        else:
            updateFeed('Build failed - ', ciJobName, ciBuildNumber, ciBuildUrl)

    except Exception as e:
        logging.exception(e)

if __name__ == '__main__':
    main(sys.argv)
