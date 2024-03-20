package models

type Chat struct {
	ID       string `json:"id,omitempty" bson:"_id,omitempty"`
	Name     string `json:"name" bson:"name"`
	ChatData Data   `json:"chatData" bson:"chatData"`
	IsGroup  bool   `json:"isGroup" bson:"isGroup"`
}
