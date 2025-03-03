# Workflow

Note that as I am the sole developer on this project, I am not doing pull requests.
However, if additional developers join the project, they should be used.

The workflow is essentially [GitHub Flow](https://githubflow.github.io/).

Tags are used to track commits tied to version updates (i.e., releases).
When an updated version has been pushed to main, tag the commit locally and remotely:

    git tag <VERSION> -m "<VERSION>"
    git push origin tag <VERSION>

Pushing a branch to the remote repository triggers a build and deploy via Jenkins.
