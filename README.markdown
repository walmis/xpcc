The GitHub Pages branch includes the project main page and the documentation.

To update the documentation or main page, you need to checkout the gh-pages branch, update the files you need and push to that branch.

These instructions should get you going (adapted from [Chris Jacob](https://gist.github.com/825950)):

You need to create the "doc/homepage" folder for your "gh-pages" branch.
If the folder has already been created by doxygen, delete it.

	user:xpcc $ cd doc
	user:doc $ mkdir homepage

Change into the "homepage" folder, clone your repo, checkout the "gh-pages" branch and then remove the "master" branch to avoid any confusion. Last step is to check that "master" branch was removed and only "gh-pages" is listed.

	user:doc $ cd homepage/
	user:homepage $ git clone git@github.com:roboterclubaachen/xpcc.git .
	user:homepage $ git checkout origin/gh-pages -b gh-pages
	user:homepage $ ls -la
	user:homepage $ git branch -d master
	user:homepage $ git branch

> You will probably get a warning when deleting the "master" branch... don't worry about it ^_^

Now you can run doxygen or modify the homepage manually.
When you want to update the changes on github, add the files, write a commit message:

	user:homepage $ git add .
	user:homepage $ git commit -m "Update documentation or project page."

Now push to the "gh-pages" branch

	user:homepage $ git push origin gh-pages

It might take a minute or two until the changes are visible on the web.
Be patient.

Change directory back to the xpcc/ folder, make sure it doesn't have the "gh-pages" branch.

	user:homepage $ cd ../../
	user:xpcc $ git branch

If the xpcc/ folder does have the "gh-pages" branch for whatever reason, change to master and remove the branch.

	user:xpcc $ git branch master
	user:xpcc $ git branch -d gh-pages
	user:xpcc $ git branch

If everything has gone well you can now work in xpcc/ and commit to "master". You can also cd into doc/homepage/, make changes and commit to "gh-pages".
