1. Two Phase Commit
> What is the need for the `COMMIT_REQUEST` message in the Two Phase Commit protocol?
>> The `COMMIT_REQUEST` message is needed to ensure that all participants are ready to commit the transaction. If a participant is not ready, it can respond with a `ABORT` message. If all participants are ready, the coordinator can send a `COMMIT` message to all participants. Once the coordinator has decided that a transaction is ready to be commited, it will ensure that every node commits the transaction. It basically helps us to ensure that all participants are ready to commit the transaction and once asked for commit, they will commit the transaction.

## Different types of failures 
> Coordinator Failure
>> What happens if the coordinator fails before sending any `COMMIT_REQUEST` message?
>> If the coordinator fails before sending the `COMMIT_REQUEST` message, the transaction will be aborted.
>>> 1. The coordinator after recovery will notice that the transaction has not even entered the prepare state. It so asks every participant to abort the transaction and waits for the ack.
>>> 2. The participants through timeout can detect that the coordinator has failed and can abort the transaction. In case the coordinator has actually not failed but message was lost, the coordinator will receive the abort message and will abort the transaction.

>> If the coordinator fails after sending the `COMMIT_REQUEST` message to some participants and before sending it to others.
>>> 1. The coordinator on recovery will notice that prepare phase was not completed. It will ask all participants to abort the transaction and wait for the ack.
>>> 2. The participants who have received the `COMMIT_REQUEST` message will wait for the message for phase 2. Through timeouts nodes that did not receive the `COMMIT_REQUEST` message can detect that the coordinator has failed and can abort the transaction.

>> If the coordinator fails after sending the `COMMIT_REQUEST` message to all participants.
>>> 1. The coordinator on recovery will notice that the prepare phase message was sent to all messages but does not know about the replies. It will ask all participants to abort the transaction and wait for the ack.
>>> 2. The participants who have received the `COMMIT_REQUEST` message will wait for the message for phase 2. 

>> If the coordinator fails without sending the `COMMIT` message but prepare phase was completed.
>>> 1. The coordinator on recovery will notice that the prepare phase was completed and a decision was made to commit the transaction. It will ask all participants to commit the transaction and wait for the ack or it can ask to abort the transaction so that locks can be released.
>>> 2. The participants will be blocked as they have not received the `COMMIT` message. This is the problem with the Two Phase Commit protocol. The participants will have to wait for the coordinator to recover and send the `COMMIT` message.

>> If the coordinator fails after sending the `COMMIT` message to some participants and before sending it to others. (Prepare phase is completed)
>>> 1. The coordinator on recovery will notice that the prepare phase was completed and a decision was made to commit the transaction. It will ask all participants to commit the transaction and wait for the ack.
>>> 2. The participants who have received the `COMMIT` message will commit the transaction. The participants who have not received the `COMMIT` message will wait for the message for phase 2. Here, if we allow communication between the participants, the participants who have committed the transaction can inform the participants who have not received the `COMMIT` message to commit the transaction. Through this they can know that the coordinator has already decided to commit the transaction.

>> If the coordinator fails after sending the `COMMIT` message to all participants.
>>> 1. The coordinator on recovery will notice that the prepare phase was completed and a decision was made to commit the transaction. It will ask all participants to commit the transaction and wait for the ack.
>>> 2. The participants who have received the `COMMIT` message will commit the transaction and send the ack to the coordinator. If the coordinator asks again, they will send the ack again. 

>>> Similar things will be done for the `ABORT` message.

> Participant Failure
>> What happens if a participant fails before receiving any `COMMIT_REQUEST` message?
>>> 1. The coordinator will wait for the ack from the participant. If the participant does not respond, the coordinator will abort the transaction via timeout.
>>> 2. The participant on recovery will notice that the transaction has not even entered the prepare state and will abort the transaction. 

>> What happens if a participant fails after receiving the `COMMIT_REQUEST` message but could not respond?

>>> 1. The coordinator will wait for the ack from the participant. If the participant does not respond, the coordinator will abort the transaction via timeout.
>>> 2. The participant on recovery will notice that the transaction has entered the prepare state but it has not been able to respond. It will abort the transaction.

>> What happens if a participant fails after sending the `YES` or `NO` message?
>>> 1. If the participant fails after sending the `YES` message, the coordinator based on the final decision will keep on sending requests to the participant. 
>>> 2. If the participant fails after sending the `NO` message, the coordinator will abort the transaction. It will ask other participants to abort the transaction. Here note that the coordinator will not wait for the ack from the participant who has failed since it has already sent the `NO` message.
>>> 3. The participant on recovery will notice that the transaction has entered the prepare state and it has responded with a `YES` or `NO` message. If it has responded with a `YES` message, it will wait for the `COMMIT` or `ABORT` message. If it has responded with a `NO` message, it need not wait for the `ABORT` message.