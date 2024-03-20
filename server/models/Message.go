package models

type Message struct {
	ID        string      `json:"id,omitempty" bson:"_id,omitempty"`
	Sender    string      `json:"sender,omitempty" bson:"sender,omitempty"`
	Type      string      `json:"type" bson:"type"`
	Recipient string      `json:"recipient,omitempty" bson:"recipient,omitempty"`
	Content   interface{} `json:"content,omitempty" bson:"content,omitempty"`
}
