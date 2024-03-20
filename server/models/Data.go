package models

type Data struct {
	Users    []User    `json:"users"`
	Messages []Message `json:"messages"`
}
