package models

type User struct {
	Username string    `json:"username" bson:"username"`
	Password string    `json:"password" bson:"password"`
	Messages []Message `json:"Messages" bson:"Messages"`
}
