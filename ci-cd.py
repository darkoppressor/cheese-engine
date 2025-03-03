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

def main (argv):
    args = argparser.parse_args()

    ciJobName = args.ciJobName
    ciBuildNumber = args.ciBuildNumber
    ciBuildUrl = args.ciBuildUrl

    try:
        updateFeed('Build started - ', ciJobName, ciBuildNumber, ciBuildUrl)

        if subprocess.run(['tools/build-system/scripts/build-engine', os.getcwd()]).returncode == 0:
            updateFeed('Build completed successfully - ', ciJobName, ciBuildNumber, ciBuildUrl)

            if subprocess.run(['tools/build-system/scripts/deploy-engine', os.getcwd()]).returncode == 0:
                updateFeed('Deployment completed successfully - ', ciJobName, ciBuildNumber, ciBuildUrl)
                sys.exit(0)
            else:
                updateFeed('Deployment failed - ', ciJobName, ciBuildNumber, ciBuildUrl)
                sys.exit(1)
        else:
            updateFeed('Build failed - ', ciJobName, ciBuildNumber, ciBuildUrl)
            sys.exit(1)

    except Exception as e:
        logging.exception(e)
        sys.exit(1)

if __name__ == '__main__':
    main(sys.argv)
