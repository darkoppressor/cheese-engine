import sys
import argparse
import logging
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

def main(argv):
    args = argparser.parse_args()

    ciJobName = args.ciJobName
    ciBuildNumber = args.ciBuildNumber
    ciBuildUrl = args.ciBuildUrl

    try:
        text = 'Build started - ' + ciJobName + ' ' + ciBuildNumber + ' (<' + ciBuildUrl + '|Open>)'
        feeds.post('jenkins', 'Wells Family Jenkins', 'Tails', 'tails.at.mobius@gmail.com', 'https://wells-family.xyz/', 'https://wells-family.xyz/favicon.png', text, text)

        subprocess.run(['chmod', '+x', 'tools/*'])
        subprocess.run(['chmod', '+x', 'tools/build-system/*'])
        subprocess.run(['chmod', '+x', 'tools/build-system/scripts/*'])
        subprocess.run(['chmod', '-x', 'tools/build-system/scripts/*-x86_64'])
        subprocess.run(['chmod', '-x', 'tools/build-system/scripts/*.cmake'])
        subprocess.run(['chmod', '+x', 'tools/build-system/scripts/android/*'])
        subprocess.run(['chmod', '+x', 'development/android/android-prep'])
        subprocess.run(['chmod', '+x', 'development/android/clean-android'])

        if subprocess.run(['tools/build-system/scripts/build-engine', '$(pwd)']):
            text = 'Build completed successfully - ' + ciJobName + ' ' + ciBuildNumber + ' (<' + ciBuildUrl + '|Open>)'
            feeds.post('jenkins', 'Wells Family Jenkins', 'Tails', 'tails.at.mobius@gmail.com', 'https://wells-family.xyz/', 'https://wells-family.xyz/favicon.png', text, text)

            if subprocess.run(['tools/build-system/scripts/deploy-engine', '$(pwd)']):
                text = 'Deployment completed successfully - ' + ciJobName + ' ' + ciBuildNumber + ' (<' + ciBuildUrl + '|Open>)'
                feeds.post('jenkins', 'Wells Family Jenkins', 'Tails', 'tails.at.mobius@gmail.com', 'https://wells-family.xyz/', 'https://wells-family.xyz/favicon.png', text, text)
            else:
                text = 'Deployment failed - ' + ciJobName + ' ' + ciBuildNumber + ' (<' + ciBuildUrl + '|Open>)'
                feeds.post('jenkins', 'Wells Family Jenkins', 'Tails', 'tails.at.mobius@gmail.com', 'https://wells-family.xyz/', 'https://wells-family.xyz/favicon.png', text, text)
        else:
            text = 'Build failed - ' + ciJobName + ' ' + ciBuildNumber + ' (<' + ciBuildUrl + '|Open>)'
            feeds.post('jenkins', 'Wells Family Jenkins', 'Tails', 'tails.at.mobius@gmail.com', 'https://wells-family.xyz/', 'https://wells-family.xyz/favicon.png', text, text)

    except Exception as e:
        logging.exception(e)

if __name__ == '__main__':
    main(sys.argv)
