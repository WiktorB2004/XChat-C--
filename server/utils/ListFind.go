package utils

import (
	"server/models"
)

func UserFind(target string, list []models.User) *models.User {
	for i := range list {
		if list[i].Username == target {
			return &list[i]
		}
	}
	return nil
}

func StringContain(elems []string, target string) bool {
	for _, s := range elems {
		if target == s {
			return true
		}
	}
	return false
}
