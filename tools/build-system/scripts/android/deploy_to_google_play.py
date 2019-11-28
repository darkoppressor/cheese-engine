import sys
import argparse
import logging

from google.oauth2 import service_account
from googleapiclient.discovery import build
from googleapiclient.http import MediaFileUpload

SCOPES = ['https://www.googleapis.com/auth/androidpublisher']

logging.basicConfig(level = logging.WARNING)

argparser = argparse.ArgumentParser(description = 'Upload an application bundle to Google Play.')
argparser.add_argument('service_account_file',
                        help = 'the path to the service account file')
argparser.add_argument('package_name',
                        help = 'the package name')
argparser.add_argument('aab_file',
                        help = 'the path to the application bundle file to upload')

def main(argv):
    args = argparser.parse_args()

    service_account_file = args.service_account_file
    package_name = args.package_name
    aab_file = args.aab_file

    credentials = service_account.Credentials.from_service_account_file(service_account_file, scopes = SCOPES)

    androidpublisher = build(serviceName = 'androidpublisher', version = 'v3', credentials = credentials)

    try:
        insert_response = androidpublisher.edits().insert(packageName = package_name, body = {}).execute()
        edit_id = insert_response['id']

        media = MediaFileUpload(aab_file, mimetype = 'application/octet-stream')

        aab_upload_response = androidpublisher.edits().bundles().upload(packageName = package_name, editId = edit_id,
                                                    media_body = media).execute()
        version_code = aab_upload_response['versionCode']

        track_update_response = androidpublisher.edits().tracks().update(packageName = package_name, editId = edit_id,
                                                                track = 'production',
                                                                body = {u'releases': [{
                                                                u'versionCodes': [str(version_code)],
                                                                u'status': u'draft'
                                                                }]}).execute()

        commit_response = androidpublisher.edits().commit(packageName = package_name, editId = edit_id).execute()

    except Exception as e:
        logging.exception(e)

if __name__ == '__main__':
    main(sys.argv)
