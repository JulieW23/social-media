#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "friends.h"

/*
 * Create a new user object with the given name.
 * Helper function for create_user.
 * Return:
 *   - Pointer to the created user object.
 */
User* create_user_obj(const char *name){
    User *x=  (User *) malloc(sizeof(User));
    strcpy(x->name, name);
    for(int i = 0; i < MAX_FRIENDS; i++){
        x->friends[i] = NULL;
    }
    x->first_post = NULL;
    x->next = NULL;
    return x;
}


/*
 * Create a new user with the given name.  Insert it at the tail of the list
 * of users whose head is pointed to by *user_ptr_add.
 *
 * Return:
 *   - 0 if successful
 *   - 1 if a user by this name already exists in this list
 *   - 2 if the given name cannot fit in the 'name' array
 *       (don't forget about the null terminator)
 */
int create_user(const char *name, User **user_ptr_add) {
    // If a user by this name already exists in this list
    if (find_user(name, *user_ptr_add) != NULL){
        return 1;
    }
    // If the given name cannot fit in the 'name' array
    if (strlen(name) >= MAX_NAME){
        return 2;
    }
    
    User *current = *user_ptr_add;
    // If there are no existing users
    if (current == NULL) {
        *user_ptr_add = create_user_obj(name);
        return 0;
    }
    // If there are already existing users
    while (current->next != NULL){
        current = current->next;
    }
    current->next = create_user_obj(name);
    return 0;
}


/*
 * Return a pointer to the user with this name in
 * the list starting with head. Return NULL if no such user exists.
 *
 * NOTE: You'll likely need to cast a (const User *) to a (User *)
 * to satisfy the prototype without warnings.
 */
User *find_user(const char *name, const User *head) {
    User *current = (User *)head;
    while (current != NULL){
        if (strcmp(current->name, name) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}


/*
 * Print the usernames of all users in the list starting at curr.
 * Names should be printed to standard output, one per line.
 */
void list_users(const User *curr) {
    printf("User LIst \n");
    while (curr != NULL){
        printf("      %s\n", curr->name);
        curr = curr->next;
    }
}


/*
 * Change the filename for the profile pic of the given user.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if the file does not exist.
 *   - 2 if the filename is too long.
 */
int update_pic(User *user, const char *filename) {
    if (strlen(filename) >= MAX_NAME){
        return 2;
    }
    
    FILE* pfile = fopen(filename, "r");
    if(pfile == NULL){
        return 1;
    }
    fclose(pfile);
    
    //memset(user->profile_pic, '\0', sizeof(user->profile_pic));
    strcpy(user->profile_pic, filename);
    return 0;
}


/*
 * Make two users friends with each other.  This is symmetric - a pointer to
 * each user must be stored in the 'friends' array of the other.
 *
 * New friends must be added in the first empty spot in the 'friends' array.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if the two users are already friends.
 *   - 2 if the users are not already friends, but at least one already has
 *     MAX_FRIENDS friends.
 *   - 3 if the same user is passed in twice.
 *   - 4 if at least one user does not exist.
 *
 * Do not modify either user if the result is a failure.
 * NOTE: If multiple errors apply, return the *largest* error code that applies.
 */
int make_friends(const char *name1, const char *name2, User *head) {
    // If the same user is passed in twice
    if(*name1 == *name2){
        return 3;
    }
    // If at least one user does not exist
    if(find_user(name1, head) == NULL || find_user(name2, head) == NULL){
        return 4;
    }
    
    User *user1 = find_user(name1, head);
    User *user2 = find_user(name2, head);
    int friends1 = 0; // number of friends that user 1 has
    int friends2 = 0; // number of friends that user 2 has
    int emptyf1 = -1; // the index where the cell in array friend[] of user1 is empty
    int emptyf2 = -1; // the index where the cell in array friend[] of user2 is empty   
    for (int i = 0; i < MAX_FRIENDS; i++){
        // count number of friends that user 1 and 2 have
        if(user1->friends[i] != NULL){
            friends1 += 1;
        }
        else {
        emptyf1 = i;
        }
        if(user2->friends[i] != NULL){
            friends2 += 1;
        }
        else {
        emptyf2 = i;
        }
        // if user 1 and 2 are already friends
        if (user1->friends[i]->name == user2->name) {
            return 1;
        }
    }
    // If the users are not already friends, but at least one already
    // MAX_FRIENDS friends
    if (friends1 == MAX_FRIENDS || friends2 == MAX_FRIENDS) {
        return 2;
    }
    
    // If there are no problems, make users friends, 
    // emptyf1 and emptyf2 are used to indicate an empty  location of user1/2 n array firend[]
    //user1->friends[friends1] = user2;
    //user2->friends[friends2] = user1;
    user1->friends[emptyf1] = user2;
    user2->friends[emptyf2] = user1;
    return 0;
}


/*
 * Prints the author, date and contents of the post pointed to 
 * by *post_pointer. 
 * Helper function for print_user.
 */
void print_post(Post *post_pointer){
    printf("From: %s\n", post_pointer->author);
    printf("Date: %s\n", asctime(gmtime(post_pointer->date)));
    printf("%s\n", post_pointer->contents);
}


/*
 * Print a user profile.
 * For an example of the required output format, see the example output
 * linked from the handout.
 * Return:
 *   - 0 on success.
 *   - 1 if the user is NULL.
 */
int print_user(const User *user) {
    // If the user is NULL
    if(user == NULL){
        return 1;
    }
    // Print profile picture
   int line_length = 200; // WHAT SHOULD MAX LINE LENGTH BE? *************
   char line[line_length + 1];
 
 
    FILE *pp = fopen(user->profile_pic, "r");
    if(pp != NULL){
        while (fgets(line, line_length+1, pp) != NULL) {
            printf("%s", line);
        }
        printf("\n\n");
        fclose(pp);
    }
    // Print name
    printf("Name: %s\n", user->name);
    printf("\n------------------------------------------\n");
    // Print friends
    printf("Friends:\n");
    for (int i = 0; i < MAX_FRIENDS; i++){
        if (user->friends[i] != NULL){
            printf("%s\n", user->friends[i]->name);
        }
    }
    printf("------------------------------------------\n");
    // Print posts
    printf("Posts:\n");
    if (user->first_post != NULL) {
        Post *post_pointer = user->first_post;
        while (post_pointer != NULL){
            if (post_pointer->next != NULL){
                print_post(post_pointer);
                printf("\n===\n\n");
            }
            else{
                print_post(post_pointer);
            }
            post_pointer = post_pointer->next;
        }
    }
    printf("------------------------------------------\n");
    return 0;
}


/*
 * Make a new post from 'author' to the 'target' user,
 * containing the given contents, IF the users are friends.
 *
 * Insert the new post at the *front* of the user's list of posts.
 *
 * Use the 'time' function to store the current time.
 *
 * 'contents' is a pointer to heap-allocated memory - you do not need
 * to allocate more memory to store the contents of the post.
 *
 * Return:
 *   - 0 on success
 *   - 1 if users exist but are not friends
 *   - 2 if either User pointer is NULL
 */
int make_post(const User *author, User *target, char *contents) {
    // If either User pointer is NULL
    if (author == NULL || target == NULL) {
        return 2;
    }
    
    for (int i = 0; i < MAX_FRIENDS; i++) {
        // Check if users are friends
        if (author->friends[i]->name == target->name) {
            // If the users are friends, create the post
            Post *p = (Post *) malloc(sizeof(Post));
            strcpy(p->author, author->name);
            p->contents = contents;
            time_t *t = (time_t *) malloc(sizeof(time_t));
            *t = time(NULL);
            p->date = t;
            // If there are no existing posts
            if (target->first_post == NULL) {
                target->first_post = p;
                return 0;
            }
            // If there are already existing posts
            else{
                p->next = target->first_post;
                target->first_post = p;
                return 0;
            }
        }
        // If users exists but are not friends
        if (i == MAX_FRIENDS - 1 && author->friends[i]->name != target->name){
            return 1;
        }
    }
    return 0;
}


/* 
 * From the list pointed to by *first_post, delete all of 
 * the posts in the list and free all of the allocated 
 * memory for these posts.
 * Helper function for delete_user.
 */
void clear_posts(Post *first_post){
    Post *curr_next = NULL;
    while (first_post != NULL){
        free(first_post->date);
        free(first_post->contents);
        curr_next = first_post->next;
        free(first_post);
        first_post = curr_next;
    }
}


/*
 * From the list pointed to by *first_user, search all of
 * the users' friend lists for the user with the given name 
 * and then if found, replace it with NULL.
 * Helper function for delete_user.
 */
void purge_friend(User* first_user, const char *name){
    while (first_user != NULL){
        for (int i = 0; i < MAX_FRIENDS; i++){
            if (first_user->friends[i] != NULL && strcmp(first_user->friends[i]->name, name) == 0){
                first_user->friends[i] = NULL;
            }
        }
        first_user = first_user->next;
    }
}


/*
 * From the list pointed to by *user_ptr_del, delete the user
 * with the given name.
 * Remove the deleted user from any lists of friends.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if a user with this name does not exist.
 */
int delete_user(const char *name, User **user_ptr_del) {
    // used to point to the user before current
    User *before_current =NULL;
    // used to find the user to be deleted
    User *current = *user_ptr_del;
    // first_user will always point to the first user in the linked list
    User *first_user = *user_ptr_del;
    
    // if the user to be deleted does not exist
    if (find_user(name, *user_ptr_del) == NULL){
        return 1;
    }
    
    // if first user in list needs to be deleted
    if (strcmp(current->name, name) == 0){
        // delete user
        *user_ptr_del = current->next;
        // deal with the user's posts
        //if the user has posts
        clear_posts(current->first_post);
//        Post *curr = current->first_post;
//        Post *curr_next;
//        while (curr != NULL){
//            free(curr->date);
//            curr_next = curr->next;
//            free(curr);
//            curr = curr_next;
//        }
        // remove user from other user's friend lists
        purge_friend(current, name);
        // free allocated memory of the first user (deleted user)
        free(first_user);
        return 0;
    }
    // if any user that is not the first user needs to be deleted
    // find the user
    // while current points to a user and the user is not the one required
    // continue searching along the linked list
    while (current != NULL && strcmp(current->name, name) != 0){
        before_current = current;
        current = current->next;
    }
    // the required user has been found
    // delete the user's posts
    //if the user has posts
    clear_posts(current->first_post);
//    Post *curr = current->first_post;
//    Post *curr_next;
//    while (curr != NULL){
//        free(curr->date);
//        curr_next = curr->next;
//        free(curr);
//        curr = curr_next;
//    }
    
    // delete user
    before_current->next = current->next;

    // remove user from other user's friend lists
    purge_friend(first_user, name);
    // free memory allocated for the deleted user
    free(current);
    return 0;
}
