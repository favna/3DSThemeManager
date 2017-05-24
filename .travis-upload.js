// Themely bleeding edge build uploader
// Copyright (c) 2017 Erman SAYIN

var GitHubApi = require("github");

var github = new GitHubApi({
	protocol: "https",
	host: "api.github.com",
	pathPrefix: "",
	headers: {
		"user-agent": "Themely bleeding edge build uploader"
	},
	followRedirects: false,
	timeout: 25000
});

// authenticate
github.authenticate({
	type: "token",
	token: process.env.bleedingEdgeRepoOAuthToken
});

// create release
github.repos.createRelease({
	owner: "ErmanSayin",
	repo: "Themely-bleeding",
	tag_name: process.env.TRAVIS_COMMIT.substr(0, 7),
	name: process.env.TRAVIS_COMMIT,
	body: process.env.TRAVIS_COMMIT_MESSAGE
}, function(err, res){
	if(err) throw err;

	var releaseId = res.data.id;

	// now upload the cia
	github.repos.uploadAsset({
		owner: "ErmanSayin",
		repo: "Themely-bleeding",
		id: releaseId,
		filePath: "output/3ds-arm/Themely.cia",
		name: "Themely.cia"
	}, function(err, res){
		if(err) throw err;

		// neat, now upload the 3dsx
		github.repos.uploadAsset({
			owner: "ErmanSayin",
			repo: "Themely-bleeding",
			id: releaseId,
			filePath: "output/3ds-arm/3ds/Themely/Themely.3dsx",
			name: "Themely.3dsx"
		}, function(err, res){
			if(err) throw err;

			// cool and good
			console.log("Success uploading bleeding edge builds.");
		});
	});
});
